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


struct MenuData {
	sf::RectangleShape backgroundShade;

	sf::View camera;
	sf::View ui;

	sf::Font font;

	sf::Text text1{font};
	sf::FloatRect text1Bounds;
	int text1Pos;

	sf::Text text2{font};
	sf::FloatRect text2Bounds;
	int text2Pos;

	sf::Text text3{font};
	sf::FloatRect text3Bounds;
	int text3Pos;

	sf::Texture pointerTexture;
	sf::Sprite pointer{pointerTexture};
	bool drawPointer;

	float halfWindowX;
	float halfWindowY;
	int uiPadding;
};

MenuData SetUpMenu(
	sf::RenderWindow& window,
	std::string text1, int text1Size, int text1Location,
	std::string text2, int text2Size, int text2Location,
	std::string text3, int text3Size, int text3Location,
	bool drawPointer, float pointerMultiplier,
	int uiPadding
);

void GenerateBackgroundSprites(GlobalData& globalData);
void HandleMenuResize(GlobalData& globalData, MenuData& menuData, sf::Vector2f newWindowSize, sf::RectangleShape& backgroundShade);

void DrawMenuBackground(GlobalData& globalData, sf::View& backgroundView, sf::View& textView, sf::RectangleShape& backgroundShade);
void DrawMenu(GlobalData& globalData, MenuData& data);

void MovePointer(MenuData& data, int selectedButton);
