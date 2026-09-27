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

#include "GlobalData.h"
#include "MenuFunctions.h"

// TODO: add export maze option?

// returns 0 to quit, 1 to restart, 2 to continue
int ShowGamePauseMenu(GlobalData& globalData)
{
	int uiPadding = 40.0f;

	MenuData data = SetUpMenu(
		globalData.window,
		"Continue", 40, 0,
		"Restart", 40, 1,
		"Return to Main Menu", 40, 2,
		true, 2,
		uiPadding
	);
	int selectedButton = 0;
	MovePointer(data, selectedButton);

	sf::Text gamePausedText(data.font, "Game Paused", 80);
	sf::FloatRect gamePausedTextBounds = gamePausedText.getLocalBounds();
	gamePausedText.setOrigin({gamePausedTextBounds.position.x + gamePausedTextBounds.size.x / 2.0f,
							  gamePausedTextBounds.position.y + gamePausedTextBounds.size.y / 2.0f});
	sf::Vector2f textPos({data.halfWindowX, data.halfWindowY - 2 * uiPadding});
	gamePausedText.setPosition(textPos);
	gamePausedTextBounds = gamePausedText.getGlobalBounds();

	while (globalData.window.isOpen())
	{
		while (const std::optional event = globalData.window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				globalData.window.close();
			if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
				sf::Vector2f mousePos = globalData.window.mapPixelToCoords({mousePressed->position.x, mousePressed->position.y});

				if (data.text1Bounds.contains(mousePos))  return 0;
				if (data.text2Bounds.contains(mousePos))  return 1;
				if (data.text3Bounds.contains(mousePos))  return 2;
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
					return selectedButton;
				}
				if (keyPressed->code == sf::Keyboard::Key::Up) {
					if (selectedButton == 0) selectedButton = 2;
					else					 selectedButton--;
					MovePointer(data, selectedButton);
				}
				if (keyPressed->code == sf::Keyboard::Key::Down) {
					if (selectedButton == 2) selectedButton = 0;
					else					 selectedButton++;
					MovePointer(data, selectedButton);
				}
				if (keyPressed->code == sf::Keyboard::Key::Escape) {
					return 0;
				}
			}
			if (const auto* windowResized = event->getIf<sf::Event::Resized>()) {
				sf::Vector2f windowSize = {static_cast<float>(windowResized->size.x), static_cast<float>(windowResized->size.y)};
				HandleMenuResize(globalData, data, windowSize, data.backgroundShade);
			}
		}

		globalData.window.clear();

		DrawMenu(globalData, data);

		globalData.window.draw(gamePausedText);

		globalData.window.display();
	}

	return 0;
}
