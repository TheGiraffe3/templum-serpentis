/*
Copyright (c) 2026 by TheGiraffe3

Templum Serpentis is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later version.

Templum Serpentis is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program. If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#include <SFML/Graphics.hpp>

#include <NFD/nfd.hpp>

#include <fstream>
#include <iostream>

#include "GameConfig.h"
#include "GlobalData.h"
#include "LoadFile.h"
#include "MenuFunctions.h"

std::string getDataFile(sf::Text& currentPath, sf::FloatRect& currentPathBounds, sf::Vector2f& currentSelectedPathPos, std::string& previousPath)
{
	NFD::Guard nfdGuard;

	NFD::UniquePath outPathNFD;

	nfdfilteritem_t filterItem[1] = {{"Text File", "txt"}};

	nfdresult_t result = NFD::OpenDialog(outPathNFD, filterItem, 1);

	if (result == NFD_OKAY) {
		std::string outPath = outPathNFD.get();
		std::vector<std::string> outPathSlashSplit = splitString(outPath, std::filesystem::path::preferred_separator);
		std::string filename = outPathSlashSplit.back();
		currentPath.setString(filename);
		currentPathBounds = currentPath.getLocalBounds();
		currentPath.setOrigin({currentPathBounds.position.x + currentPathBounds.size.x / 2.0f,
							   currentPathBounds.position.y + currentPathBounds.size.y / 2.0f});
		currentPath.setPosition(currentSelectedPathPos);
		currentPathBounds = currentPath.getGlobalBounds();
		return outPath;
	} else if (result == NFD_CANCEL) {
		// do nothing; previousPath is returned
	} else {
		std::cout << "Error: " << NFD::GetError() << std::endl;
	}

	return previousPath;
}

void handleDataFileFailure(sf::Text& currentPath, sf::FloatRect& currentPathBounds, sf::Vector2f& currentSelectedPathPos)
{
	currentPath.setString("Maze failed to load. Please try a different file.");
	currentPathBounds = currentPath.getLocalBounds();
	currentPath.setOrigin({currentPathBounds.position.x + currentPathBounds.size.x / 2.0f,
							currentPathBounds.position.y + currentPathBounds.size.y / 2.0f});
	currentPath.setPosition(currentSelectedPathPos);
	currentPathBounds = currentPath.getGlobalBounds();

	return;
}

void ShowLoadMazeMenu(GlobalData& globalData, GameCfg::Config& gameConfig)
{
	int uiPadding = 40;

	MenuData data = SetUpMenu(
		globalData.window,
		"Open File", 50, -2,
		"Save Config", 40, 1,
		"Cancel", 40, 2,
		true, 2,
		uiPadding
	);
	int selectedButton = 0;
	MovePointer(data, selectedButton);

	sf::Text currentSelectedPath(data.font, "no file selected", 40);
	sf::FloatRect currentSelectedPathBounds = currentSelectedPath.getLocalBounds();
	currentSelectedPath.setOrigin({currentSelectedPathBounds.position.x + currentSelectedPathBounds.size.x / 2.0f,
								   currentSelectedPathBounds.position.y + currentSelectedPathBounds.size.y / 2.0f});
	sf::Vector2f currentSelectedPathPos({data.halfWindowX, data.halfWindowY - uiPadding});
	currentSelectedPath.setPosition(currentSelectedPathPos);
	currentSelectedPathBounds = currentSelectedPath.getGlobalBounds();

	std::string filePath;
	bool getDataFileNow = false;

	while (globalData.window.isOpen())
	{
		if (getDataFileNow == true) {
			filePath = getDataFile(currentSelectedPath, currentSelectedPathBounds, currentSelectedPathPos, filePath);
			getDataFileNow = false;
		}

		while (const std::optional event = globalData.window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				globalData.window.close();
			if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
				sf::Vector2f mousePos = globalData.window.mapPixelToCoords({mousePressed->position.x, mousePressed->position.y});

				if (data.text1Bounds.contains(mousePos)) {
					getDataFileNow = true;
				}
				if (data.text2Bounds.contains(mousePos)) {
					bool itWorks = loadDataFile(filePath, gameConfig);
					if (itWorks == false) {
						handleDataFileFailure(currentSelectedPath, currentSelectedPathBounds, currentSelectedPathPos);
						break;
					}
					return;
				}
				if (data.text3Bounds.contains(mousePos))	return;
			}
			if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>())
			{
				sf::Vector2f mousePos = globalData.window.mapPixelToCoords({mouseMoved->position.x, mouseMoved->position.y});

				if (data.text1Bounds.contains(mousePos)) {
					selectedButton = 0;
					MovePointer(data, selectedButton);
				}
				if (data.text2Bounds.contains(mousePos)) {
					selectedButton = 1;
					MovePointer(data, selectedButton);
				}
				if (data.text3Bounds.contains(mousePos)) {
					selectedButton = 2;
					MovePointer(data, selectedButton);
				}
			}
			if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
				if (keyPressed->code == sf::Keyboard::Key::Enter) {
					if (selectedButton == 0) {
						getDataFileNow = true;
					}
					if (selectedButton == 1) {
						bool itWorks = loadDataFile(filePath, gameConfig);
						if (itWorks == false) {
							handleDataFileFailure(currentSelectedPath, currentSelectedPathBounds, currentSelectedPathPos);
							break;
						}
						return;
					}
					if (selectedButton == 2)	return;
				}
				if (keyPressed->code == sf::Keyboard::Key::Up) {
					if (selectedButton > 0)	 selectedButton--;
					else	selectedButton = 2;
					MovePointer(data, selectedButton);
				}
				if (keyPressed->code == sf::Keyboard::Key::Down) {
					if (selectedButton < 2)	 selectedButton++;
					else	selectedButton = 0;
					MovePointer(data, selectedButton);
				}
				if (keyPressed->code == sf::Keyboard::Key::Escape) {
					return;
				}
			}
			if (const auto* windowResized = event->getIf<sf::Event::Resized>()) {
				sf::Vector2f windowSize = {static_cast<float>(windowResized->size.x), static_cast<float>(windowResized->size.y)};
				HandleMenuResize(globalData, data, windowSize, data.backgroundShade);
			}
		}

		globalData.window.clear();

		DrawMenu(globalData, data);

		globalData.window.draw(currentSelectedPath);

		globalData.window.display();
	}

	return;
}
