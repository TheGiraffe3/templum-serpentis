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

#include "random.h"

#include "GlobalData.h"
#include "MenuFunctions.h"

std::string getRandomVictoryMessage()
{
	const std::vector<std::string> victoryMessages = {
		"Nice job making it through the door!",
		"Nice job making it through the door!",
		"Nice job making it through the door!",
		"You made it through the door!",
		"Awesome!",
		"Nice!",
		"Gold may be disposed of at the nearest retailer.",
		"Per labyrinthum reptasti! Awesomo!",
	};
	int number = getRandomNumber(0, victoryMessages.size() - 1);
	return victoryMessages[number];
}

std::string getRandomDefeatMessage()
{
	const std::vector<std::string> defeatMessages = {
		"Avoid the serpents next time!",
		"Avoid the serpents next time!",
		"Avoid the serpents next time!",
		"Close!",
		"Serpentes vitare! Oopsum!",
		"Almost there! Better luck next time!",
	};
	int number = getRandomNumber(0, defeatMessages.size() - 1);
	return defeatMessages[number];
}

// parameters - 0 = player loses, 1 = player wins
void ShowGameOverMenu(GlobalData& globalData, int endConditions)
{
	std::string victoryText = "You win!";
	if (endConditions == 0)   victoryText = "You lose!";

	std::string explanationText = getRandomVictoryMessage();
	if (endConditions == 0)	 explanationText = getRandomDefeatMessage();

	MenuData menuData = SetUpMenu(
		globalData.window,
		"Game Over!", 80, -2,
		victoryText, 40, 0,
		explanationText, 40, 1,
		false, 2,
		40
	);

	int framesSoFar = 90;

	while (globalData.window.isOpen())
	{
		while (const std::optional event = globalData.window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				globalData.window.close();
			if (framesSoFar < 1 && (event->getIf<sf::Event::MouseButtonPressed>() || event->getIf<sf::Event::KeyPressed>())) {
				return;
			}
			if (const auto* windowResized = event->getIf<sf::Event::Resized>()) {
				sf::Vector2f windowSize = {static_cast<float>(windowResized->size.x), static_cast<float>(windowResized->size.y)};
				menuData.ui = sf::View(sf::FloatRect({0.f, 0.f}, windowSize));
				menuData.camera = sf::View(sf::FloatRect({0.f, 0.f}, windowSize));
				menuData.camera.setCenter({menuData.camera.getCenter().x + 16, menuData.camera.getCenter().y + 16});
			}
			if (const auto* windowResized = event->getIf<sf::Event::Resized>()) {
				sf::Vector2f windowSize = {static_cast<float>(windowResized->size.x), static_cast<float>(windowResized->size.y)};
				HandleMenuResize(globalData, menuData, windowSize, menuData.backgroundShade);
			}
		}

		if (framesSoFar > 0)	framesSoFar--;

		globalData.window.clear();

		DrawMenu(globalData, menuData);

		globalData.window.display();
	}

	return;
}
