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

#include "Game.h"
#include "GlobalData.h"
#include "MenuFunctions.h"
#include "ResourcePath.hpp"

// return 1 to start a game, 2 to load a file, and 3 to quit application
int ShowMainMenu(GlobalData& globalData)
{
	MenuData data = SetUpMenu(
		globalData.window,
		"Start Game", 80, -1,
		"Load Maze", 80, 0,
		"Quit", 80, 1,
		true, 3,
		80
	);
	int selectedButton = 0;
	MovePointer(data, selectedButton);

	while (globalData.window.isOpen())
	{
		while (const std::optional event = globalData.window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				globalData.window.close();
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
			if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
				sf::Vector2f mousePos = globalData.window.mapPixelToCoords({mousePressed->position.x, mousePressed->position.y});

				if (data.text1Bounds.contains(mousePos))
					return 1;
				if (data.text2Bounds.contains(mousePos))
					return 2;
				if (data.text3Bounds.contains(mousePos))
					return 3;
			}
			if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
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
				if (keyPressed->code == sf::Keyboard::Key::Enter) {
					return selectedButton + 1;
				}
			}
			if (const auto* windowResized = event->getIf<sf::Event::Resized>()) {
				sf::Vector2f windowSize = {static_cast<float>(windowResized->size.x), static_cast<float>(windowResized->size.y)};
				HandleMenuResize(globalData, data, windowSize, data.backgroundShade);
			}
		}

		globalData.window.clear();

		DrawMenu(globalData, data);

		globalData.window.display();
	}

	return 0;
}