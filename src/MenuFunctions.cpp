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

#include <SFML/Graphics.hpp>

#include "Game.h"
#include "GlobalData.h"
#include "MenuFunctions.h"
#include "ResourcePath.hpp"

// set tilesize values
// any update to this should also change definitions in Game.cpp
const int TILESIZE = 32;

// local functions
void handleCenteringText(sf::Text& text, sf::FloatRect& textBounds, int location, int uiPadding, float halfWindowX, float halfWindowY)
{
	textBounds = text.getLocalBounds();
	text.setOrigin({textBounds.position.x + textBounds.size.x / 2.0f,
					textBounds.position.y + textBounds.size.y / 2.0f});
	sf::Vector2f textPos({halfWindowX, halfWindowY + location * uiPadding});
	text.setPosition(textPos);
	textBounds = text.getGlobalBounds();
}

int getIdealOffset(sf::RenderWindow& window, bool x)
{
	if (x == true) {
		int xOffset = (TILESIZE - window.getSize().x % TILESIZE) / 2;
		return xOffset;
	}

	int yOffset = (TILESIZE - window.getSize().y % TILESIZE) / 2;
	return yOffset;
}



MenuData SetUpMenu(
	sf::RenderWindow& window,
	std::string text1text, int text1Size, int text1Location,
	std::string text2text, int text2Size, int text2Location,
	std::string text3text, int text3Size, int text3Location,
	bool drawPointer, float pointerMultiplier,
	int uiPadding
) {
	MenuData data;

	float halfWindowX = window.getSize().x / 2;
	float halfWindowY = window.getSize().y / 2;
	data.halfWindowX = halfWindowX;
	data.halfWindowY = halfWindowY;

	std::string path = getResourcesPath();
	sf::Font font(path + "font.ttf");
	font.setSmooth(false);
	data.font = font;

	sf::RectangleShape backgroundShade(sf::Vector2f(window.getSize().x, window.getSize().y));
	backgroundShade.setFillColor(sf::Color(0, 50, 10, 150));
	backgroundShade.setPosition({
		backgroundShade.getPosition().x + getIdealOffset(window, true),
		backgroundShade.getPosition().y + getIdealOffset(window, false)
	});
	data.backgroundShade = backgroundShade;

	sf::Vector2f windowSize(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y));
	sf::View camera(sf::FloatRect({0.f, 0.f}, windowSize));
	sf::View ui(sf::FloatRect({0.f, 0.f}, windowSize));
	camera.setCenter({
		camera.getCenter().x + getIdealOffset(window, true),
		camera.getCenter().y + getIdealOffset(window, false)
	});
	data.camera = camera;
	data.ui = ui;

	sf::Text text1(data.font, text1text, text1Size);
	sf::FloatRect text1Bounds;
	handleCenteringText(text1, text1Bounds, text1Location, uiPadding, halfWindowX, halfWindowY);
	data.text1 = text1;
	data.text1Pos = text1Location;

	sf::Text text2(data.font, text2text, text2Size);
	sf::FloatRect text2Bounds;
	handleCenteringText(text2, text2Bounds, text2Location, uiPadding, halfWindowX, halfWindowY);
	data.text2 = text2;
	data.text2Pos = text2Location;

	sf::Text text3(data.font, text3text, text3Size);
	sf::FloatRect text3Bounds;
	handleCenteringText(text3, text3Bounds, text3Location, uiPadding, halfWindowX, halfWindowY);
	data.text3 = text3;
	data.text3Pos = text3Location;

	data.text1Bounds = text1.getGlobalBounds();
	data.text2Bounds = text2.getGlobalBounds();
	data.text3Bounds = text3.getGlobalBounds();

	sf::Texture pointerTexture(getResourcesPath() + "pointer.png");
	data.pointerTexture = pointerTexture;
	sf::Sprite pointer(data.pointerTexture);
	pointer.setScale({pointerMultiplier, pointerMultiplier});
	pointer.setColor(sf::Color(255,255,255,150));
	data.pointer = pointer;
	data.drawPointer = drawPointer;

	data.uiPadding = uiPadding;

	return data;
}



void GenerateBackgroundSprites(GlobalData& globalData)
{
	int windowSizeX = globalData.window.getSize().x;
	int windowSizeY = globalData.window.getSize().y;

	// create a maze for all menus to use
	// with this, we theoretically have the right maze size on all screen dimensions
	int necessaryMazeWidth = ((windowSizeX + 31) / TILESIZE) + (((windowSizeX + 31) / TILESIZE) % 2 == 0);
	int necessaryMazeHeight = ((windowSizeY + 31) / TILESIZE) + (((windowSizeY + 31) / TILESIZE) % 2 == 0);

	// have our game object create the maze
	std::pair<std::vector<sf::Sprite>, std::vector<sf::Sprite>> menuBackgroundMaze = globalData.game.generateAndDrawWallsAndGround(necessaryMazeWidth, necessaryMazeHeight);
	std::vector<sf::Sprite> menuBackgroundSprites = menuBackgroundMaze.first;
	menuBackgroundSprites.insert(menuBackgroundSprites.end(), menuBackgroundMaze.second.begin(), menuBackgroundMaze.second.end());

	globalData.backgroundSprites = menuBackgroundSprites;

	return;
}

void HandleMenuResize(GlobalData& globalData, MenuData& menuData, sf::Vector2f newWindowSize, sf::RectangleShape& backgroundShade)
{
	int newWindowSizeX = newWindowSize.x;
	int newWindowSizeY = newWindowSize.y;

	unsigned int windowX = ((newWindowSizeX + 31) / TILESIZE) + (((newWindowSizeX + 31) / TILESIZE) % 2 == 1);
	unsigned int windowY = ((newWindowSizeY + 31) / TILESIZE) + (((newWindowSizeY + 31) / TILESIZE) % 2 == 1);

	sf::Vector2u ourNewWindowSize = {windowX * TILESIZE, windowY * TILESIZE};
	if (newWindowSizeX != windowX * TILESIZE && newWindowSizeY != windowY * TILESIZE) {
		globalData.window.setSize(ourNewWindowSize);
	}

	GenerateBackgroundSprites(globalData);

	menuData.halfWindowX = globalData.window.getSize().x / 2;
	menuData.halfWindowY = globalData.window.getSize().y / 2;

	backgroundShade = sf::RectangleShape(sf::Vector2f(globalData.window.getSize().x, globalData.window.getSize().y));
	backgroundShade.setFillColor(sf::Color(0, 50, 10, 150));
	backgroundShade.setPosition({
		backgroundShade.getPosition().x + getIdealOffset(globalData.window, true),
		backgroundShade.getPosition().y + getIdealOffset(globalData.window, false)
	});

	menuData.ui = sf::View(sf::FloatRect({0.f, 0.f}, newWindowSize));

	menuData.camera = sf::View(sf::FloatRect({0.f, 0.f}, newWindowSize));
	menuData.camera.setCenter({
		menuData.camera.getCenter().x + getIdealOffset(globalData.window, true),
		menuData.camera.getCenter().y + getIdealOffset(globalData.window, false)
	});

	handleCenteringText(menuData.text1, menuData.text1Bounds, menuData.text1Pos, menuData.uiPadding, menuData.halfWindowX, menuData.halfWindowY);
	handleCenteringText(menuData.text2, menuData.text2Bounds, menuData.text2Pos, menuData.uiPadding, menuData.halfWindowX, menuData.halfWindowY);
	handleCenteringText(menuData.text3, menuData.text3Bounds, menuData.text3Pos, menuData.uiPadding, menuData.halfWindowX, menuData.halfWindowY);
	MovePointer(menuData, 0);

	return;
}



void DrawMenuBackground(GlobalData& globalData, sf::View& backgroundView, sf::View& textView, sf::RectangleShape& backgroundShade)
{
	globalData.window.setView(backgroundView);

	for (const auto& backgroundSprite : globalData.backgroundSprites) {
		globalData.window.draw(backgroundSprite);
	}

	globalData.window.draw(backgroundShade);

	globalData.window.setView(textView);

	return;
}


void DrawMenu(GlobalData& globalData, MenuData& data)
{
	DrawMenuBackground(globalData, data.camera, data.ui, data.backgroundShade);

	globalData.window.draw(data.text1);
	globalData.window.draw(data.text2);
	globalData.window.draw(data.text3);
	if (data.drawPointer)
		globalData.window.draw(data.pointer);
}



// checklist for adding a new button:
// - add the text object
// - tell the window to draw it
// - update this function
// - update all calls of this function to pass the new FloatRect
// - update mouseMoved and mousePressed functions
void MovePointer(MenuData& data, int selectedButton) {
	if (selectedButton == 0) {
		data.pointer.setPosition({
			data.text1Bounds.position.x - data.pointer.getGlobalBounds().size.x - 10,
			data.text1Bounds.position.y + (data.text1Bounds.size.y / 2) - (data.pointer.getGlobalBounds().size.y / 2)
		});
	}
	if (selectedButton == 1) {
		data.pointer.setPosition({
			data.text2Bounds.position.x - data.pointer.getGlobalBounds().size.x - 10,
			data.text2Bounds.position.y + (data.text2Bounds.size.y / 2) - (data.pointer.getGlobalBounds().size.y / 2)
		});
	}
	if (selectedButton == 2) {
		data.pointer.setPosition({
			data.text3Bounds.position.x - data.pointer.getGlobalBounds().size.x - 10,
			data.text3Bounds.position.y + (data.text3Bounds.size.y / 2) - (data.pointer.getGlobalBounds().size.y / 2)
		});
	}
	return;
}
