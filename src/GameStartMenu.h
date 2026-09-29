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

#include "GameConfig.h"
#include "GlobalData.h"
#include "LoadFile.h"
#include "MenuFunctions.h"
#include "ResourcePath.hpp"

// We define a custom function here because we have five clickable buttons.
void ShowGameStartMenuMovePointer(MenuData& data, int selectedButton, sf::FloatRect& text0Bounds, sf::FloatRect& text4Bounds) {
	if (selectedButton == 0) {
		data.pointer.setPosition({
			text0Bounds.position.x - data.pointer.getGlobalBounds().size.x - 10,
			text0Bounds.position.y + (text0Bounds.size.y / 2) - (data.pointer.getGlobalBounds().size.y / 2)
		});
	}
	if (selectedButton == 1) {
		data.pointer.setPosition({
			data.text1Bounds.position.x - data.pointer.getGlobalBounds().size.x - 10,
			data.text1Bounds.position.y + (data.text1Bounds.size.y / 2) - (data.pointer.getGlobalBounds().size.y / 2)
		});
	}
	if (selectedButton == 2) {
		data.pointer.setPosition({
			data.text2Bounds.position.x - data.pointer.getGlobalBounds().size.x - 10,
			data.text2Bounds.position.y + (data.text2Bounds.size.y / 2) - (data.pointer.getGlobalBounds().size.y / 2)
		});
	}
	if (selectedButton == 3) {
		data.pointer.setPosition({
			data.text3Bounds.position.x - data.pointer.getGlobalBounds().size.x - 10,
			data.text3Bounds.position.y + (data.text3Bounds.size.y / 2) - (data.pointer.getGlobalBounds().size.y / 2)
		});
	}
	if (selectedButton == 4) {
		data.pointer.setPosition({
			text4Bounds.position.x - data.pointer.getGlobalBounds().size.x - 10,
			text4Bounds.position.y + (text4Bounds.size.y / 2) - (data.pointer.getGlobalBounds().size.y / 2)
		});
	}
	return;
}

void ShowGameStartMenu(GlobalData& globalData, GameCfg::Config& config, bool& returnImmediately)
{
	const std::string easyPath = getResourcesPath() + "maps/easy.txt";
	const std::string mediumPath = getResourcesPath() + "maps/default.txt";
	const std::string hardPath = getResourcesPath() + "maps/hard.txt";
	const int uiPadding = 40;

	MenuData data = SetUpMenu(
		globalData.window,
		"Easy", 40, -1,
		"Medium", 40, 0,
		"Hard", 40, 1,
		true, 2,
		uiPadding
	);

	sf::Text startText(data.font, "Start Game", 80);
	sf::FloatRect startTextBounds = startText.getLocalBounds();
	startText.setOrigin({startTextBounds.position.x + startTextBounds.size.x / 2.0f,
						 startTextBounds.position.y + startTextBounds.size.y / 2.0f});
	sf::Vector2f textPos({data.halfWindowX, data.halfWindowY - 4 * uiPadding});
	startText.setPosition(textPos);
	startTextBounds = startText.getGlobalBounds();

	sf::Text currentText(data.font, "Current Config", 40);
	sf::FloatRect currentTextBounds = currentText.getLocalBounds();
	currentText.setOrigin({currentTextBounds.position.x + currentTextBounds.size.x / 2.0f,
						   currentTextBounds.position.y + currentTextBounds.size.y / 2.0f});
	textPos = {data.halfWindowX, data.halfWindowY - 2 * uiPadding};
	currentText.setPosition(textPos);
	currentTextBounds = currentText.getGlobalBounds();

	sf::Text cancelText(data.font, "Cancel", 40);
	sf::FloatRect cancelTextBounds = cancelText.getLocalBounds();
	cancelText.setOrigin({cancelTextBounds.position.x + cancelTextBounds.size.x / 2.0f,
						  cancelTextBounds.position.y + cancelTextBounds.size.y / 2.0f});
	textPos = {data.halfWindowX, data.halfWindowY + 3 * uiPadding - static_cast<float>(0.5 * uiPadding)};
	cancelText.setPosition(textPos);
	cancelTextBounds = cancelText.getGlobalBounds();

	int selectedButton = 0;
	ShowGameStartMenuMovePointer(data, selectedButton, currentTextBounds, cancelTextBounds);

	while (globalData.window.isOpen())
	{
		while (const std::optional event = globalData.window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				globalData.window.close();
			if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
				sf::Vector2f mousePos = globalData.window.mapPixelToCoords({mousePressed->position.x, mousePressed->position.y});

				if (currentTextBounds.contains(mousePos))
				{
					return;
				}
				if (data.text1Bounds.contains(mousePos))
				{
					loadDataFile(easyPath, config);
					return;
				}
				if (data.text2Bounds.contains(mousePos))
				{
					loadDataFile(mediumPath, config);
					return;
				}
				if (data.text3Bounds.contains(mousePos))
				{
					loadDataFile(hardPath, config);
					return;
				}
				if (cancelTextBounds.contains(mousePos))
				{
					returnImmediately = true;
					return;
				}
			}
			if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>())
			{
				sf::Vector2f mousePos = globalData.window.mapPixelToCoords({mouseMoved->position.x, mouseMoved->position.y});

				if (currentTextBounds.contains(mousePos)) {
					selectedButton = 0;
					ShowGameStartMenuMovePointer(data, selectedButton, currentTextBounds, cancelTextBounds);
				}
				if (data.text1Bounds.contains(mousePos)) {
					selectedButton = 1;
					ShowGameStartMenuMovePointer(data, selectedButton, currentTextBounds, cancelTextBounds);
				}
				if (data.text2Bounds.contains(mousePos)) {
					selectedButton = 2;
					ShowGameStartMenuMovePointer(data, selectedButton, currentTextBounds, cancelTextBounds);
				}
				if (data.text3Bounds.contains(mousePos)) {
					selectedButton = 3;
					ShowGameStartMenuMovePointer(data, selectedButton, currentTextBounds, cancelTextBounds);
				}
				if (cancelTextBounds.contains(mousePos)) {
					selectedButton = 4;
					ShowGameStartMenuMovePointer(data, selectedButton, currentTextBounds, cancelTextBounds);
				}
			}
			if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
				if (keyPressed->code == sf::Keyboard::Key::Enter) {
					if (selectedButton == 0)
						return;
					if (selectedButton == 1)
					{
						loadDataFile(easyPath, config);
						return;
					}
					if (selectedButton == 2)
					{
						loadDataFile(mediumPath, config);
						return;
					}
					if (selectedButton == 3)
					{
						loadDataFile(hardPath, config);
						return;
					}
					if (selectedButton == 4)
					{
						returnImmediately = true;
						return;
					}
				}
				if (keyPressed->code == sf::Keyboard::Key::Up) {
					if (selectedButton == 0) selectedButton = 4;
					else					 selectedButton--;
					ShowGameStartMenuMovePointer(data, selectedButton, currentTextBounds, cancelTextBounds);
				}
				if (keyPressed->code == sf::Keyboard::Key::Down) {
					if (selectedButton == 4) selectedButton = 0;
					else					 selectedButton++;
					ShowGameStartMenuMovePointer(data, selectedButton, currentTextBounds, cancelTextBounds);
				}
				if (keyPressed->code == sf::Keyboard::Key::Escape) {
					returnImmediately = true;
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

		globalData.window.draw(startText);
		globalData.window.draw(currentText);
		globalData.window.draw(cancelText);

		globalData.window.display();
	}

	return;
}
