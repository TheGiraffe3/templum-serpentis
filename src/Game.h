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

#include <map>
#include <vector>

#include "GameConfig.h"

struct GlobalData;

// InitializeAndRunGame returns 0 if player wins, 1 if player loses
class Game {
public:
	void shutDownGame();
	std::pair<std::vector<sf::Sprite>, std::vector<sf::Sprite>> generateAndDrawWallsAndGround(int customWidth = 45, int customHeight = 45);
	int InitializeAndRunGame(GlobalData& globalData, GameCfg::Config& config);

private:
	static constexpr int WIDTH = 45;
	static constexpr int HEIGHT = 45;

	GameCfg::Config config;

	std::vector<sf::Sprite> groundSprites;
	std::vector<sf::Sprite> serpentSprites;
	std::vector<sf::Sprite> specialSprites;
	std::vector<sf::Sprite> wallSprites;

	std::map<std::string, sf::Texture> groundTextures;
	std::map<std::string, sf::Texture> wallTextures;
	std::map<std::string, sf::Texture> specialTextures;

	void loadWallsAndGroundResources();
	void loadResources();
	sf::Texture& getResource(const std::string& name, int type);
	bool hasWall(int X, int Y);
	bool hasEntity(int X, int Y);
	int texturingWallType(int X, int Y);
	bool hasSpecialItem(int X, int Y);
	int getSpecialItem(int X, int Y);
	bool snakeInSquare(int X, int Y);
	int straightLineBetweenPoints(int X1, int Y1, int X2, int Y2);
	std::pair<int, int> getRandomUnoccupiedSpace();
	std::pair<int, int> getRandomAdjacentSpace(int X, int Y);
	int directionToMoveForRandomAdjacentSpace(int X, int Y);
	void setUpTileArrays();
	std::pair<std::vector<sf::Sprite>, std::vector<sf::Sprite>> generateSpecialItemAndSerpentSprites();
	void switchSerpentSprite(sf::Sprite& serpentSprite, int direction);
	void moveSerpent(sf::Sprite& serpentSprite);
	void moveSerpents();
	void updatePlayerSprite(sf::Sprite& playerSprite);
	void addAutoGenMazeToTiles();
	void reset();
};