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

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "Candle/RadialLight.hpp"
#include "Candle/LightingArea.hpp"
#include "mazegen.hpp"

#include <cmath>
#include <iostream>
#include <vector>

#include "random.h"

#include "Game.h"
#include "GameConfig.h"
#include "GameOverMenu.h"
#include "GamePauseMenu.h"
#include "GameStartMenu.h"
#include "GlobalData.h"
#include "ResourcePath.hpp"

// self-explanatory. since all images are 32x32 this shouldn't be changed
// without a texture pack... hmm...
// any update to this should also be made in MenuFunctions.cpp
const float TILESIZE = 32;

void Game::loadWallsAndGroundResources()
{
	std::string path = getResourcesPath();

	for (int i = 0; i <= 11; i++) {
		std::string fileName = "ground" + std::to_string(i);

		if (!groundTextures[fileName].loadFromFile(path + "ground/" + fileName + ".png"))
			throw std::runtime_error("Failed to load a ground tile.");
	}

	const std::vector<std::string> walls = {
		"wall0000", "wall0001", "wall0010", "wall0100", "wall0101",
		"wall0110", "wall0111", "wall00110", "wall00111", "wall1000",
		"wall1001", "wall1010", "wall1011", "wall1101", "wall1110",
		"wall1111", "wall11000", "wall11001", "wall2121", "wall0211",
		"wall1102", "wall1112", "wall1120", "wall1121", "wall1211",
		"wall1212", "wall1221", "wall2011", "wall2111", "wall2112"
	};
	for (const auto& wall : walls) {
		if (!wallTextures[wall].loadFromFile(path + "walls/" + wall + ".png"))
			throw std::runtime_error("Failed to load a wall tile.");
	}
}

void Game::loadResources()
{
	std::string path = getResourcesPath();

	loadWallsAndGroundResources();

	const std::vector<std::string> otherTextures = {
		"door",
		"machete", "flashlight", "gold", "key",
		"player-l", "player-r", "serpent-l", "serpent-r",
		"gold-grey", "key-grey"
	};
	const std::vector<std::string> playerTextures = {
		"player000", "player001", "player010", "player011",
		"player100", "player101", "player110", "player111"
	};
	for (const auto& texture : otherTextures) {
		if (!specialTextures[texture].loadFromFile(path + texture + ".png"))
			throw std::runtime_error("Failed to load a sprite.");
	}
	for (const auto& player : playerTextures) {
		if (!specialTextures[player].loadFromFile(path + "player/" + player + ".png"))
			throw std::runtime_error("Failed to load a player sprite.");
	}
}

// 0 = ground, 1 == wall, 2 == other
sf::Texture& Game::getResource(const std::string& name, int type)
{
	if (type == 0) {
		auto it = groundTextures.find(name);
		if (it == groundTextures.end()) {
			throw std::runtime_error("Failed to get tile " + name);
		}
		return it->second;
	}
	else if (type == 1) {
		auto it = wallTextures.find(name);
		if (it == wallTextures.end()) {
			throw std::runtime_error("Failed to get tile " + name);
		}
		return it->second;
	}
	else {
		auto it = specialTextures.find(name);
		if (it == specialTextures.end()) {
			throw std::runtime_error("Failed to get special texture " + name);
		}
		return it->second;
	}
}

bool Game::hasWall(int X, int Y)
{
	if (X >= config.width || X < 0 || Y >= config.height || Y < 0) return false;
	if (config.tiles[X][Y] == 1)  return true;

	return false;
}

bool Game::hasEntity(int X, int Y)
{
	if (X >= config.width || X < 0 || Y >= config.height || Y < 0) return false;
	if (config.tiles[X][Y] == 2 || config.tiles[X][Y] == 3)  return true;

	return false;
}

int Game::texturingWallType(int X, int Y)
{
	if (X < 0 || X >= config.width || Y < 0 || Y >= config.height)  return 2;
	if (hasWall(X, Y))	  return 1;
	return 0;
}

bool Game::hasSpecialItem(int X, int Y)
{
	if (config.specialItems[X][Y] >= 1) return true;

	return false;
}

int Game::getSpecialItem(int X, int Y)
{
	return (config.specialItems[X][Y]);
}

bool Game::snakeInSquare(int X, int Y)
{
	if (config.tiles[X][Y] == 3) return true;

	return false;
}

// We return 0 if there's none, 1 if up, 2 if down, 3 if left, 4 if right
int Game::straightLineBetweenPoints(int X1, int Y1, int X2, int Y2)
{
	if (X1 == X2) {
		int yStart = std::min(Y1, Y2);
		int yEnd = std::max(Y1, Y2);

		for (int i = yStart; i < yEnd; i++) {
			if (hasWall(X1, i)) return 0;
		}

		if (Y1 < Y2) return 1;
		return 2;
	}
	if (Y1 == Y2) {
		int xStart = std::min(X1, X2);
		int xEnd = std::max(X1, X2);

		for (int i = xStart; i < xEnd; i++) {
			if (hasWall(i, Y1)) return 0;
		}

		if (X1 < X2) return 3;
		return 4;
	}
	return 0;
}

std::pair<int, int> Game::getRandomUnoccupiedSpace()
{
	bool foundEmptyArea = false;
	int X = 0;
	int Y = 0;

	while (foundEmptyArea == false) {
		X = getRandomNumber(2, config.width - 2);
		Y = getRandomNumber(2, config.height - 2);
		foundEmptyArea = !hasWall(X, Y) && !hasSpecialItem(X, Y) && !hasEntity(X, Y);
	}

	return {X, Y};
}

std::pair<int, int> Game::getRandomAdjacentSpace(int X, int Y)
{
	int direction;
	int newX;
	int newY;
	do {
		direction =  getRandomNumber(1,4);
		newX = X;
		newY = Y;
		if (direction == 1)	 newY--;
		if (direction == 2)	 newY++;
		if (direction == 3)	 newX--;
		if (direction == 4)	 newX++;
	} while (hasWall(newX, newY));
	return {newX, newY};
}

int Game::directionToMoveForRandomAdjacentSpace(int X, int Y)
{
	int direction;
	int newX;
	int newY;
	do {
		direction =  getRandomNumber(1,4);
		newX = X;
		newY = Y;
		if (direction == 1)	 newY--;
		if (direction == 2)	 newY++;
		if (direction == 3)	 newX--;
		if (direction == 4)	 newX++;
	} while (hasWall(newX, newY));
	return direction;
}

void Game::setUpTileArrays()
{
	// clear all previous items, since we were for some reason seeing duplicate doors
	// and other items
	config.specialItems.assign(config.width, std::vector<int>(config.height, 0));

	for (int y = 0; y < config.MACHETE_COUNT; y++) {
		std::pair<int, int> location = getRandomUnoccupiedSpace();
		int X = location.first;
		int Y = location.second;
		config.specialItems[X][Y] = 1;
	}

	if (config.STARTS_WITH_LIGHT == false && config.LIGHT_ENABLED == true) {
		{
			std::pair<int, int> location = getRandomUnoccupiedSpace();
			int X = location.first;
			int Y = location.second;
			config.specialItems[X][Y] = 2;
		}
	}

	for (int y = 0; y < config.GOLD_COUNT; y++) {
		std::pair<int, int> location = getRandomUnoccupiedSpace();
		int X = location.first;
		int Y = location.second;
		config.specialItems[X][Y] = 3;
	}

	if (config.STARTS_WITH_KEY == false) {
		{
			std::pair<int, int> location = getRandomUnoccupiedSpace();
			int X = location.first;
			int Y = location.second;
			config.specialItems[X][Y] = 4;
		}
	}

	{
		std::pair<int, int> location = getRandomUnoccupiedSpace();
		int X = location.first;
		int Y = location.second;
		config.specialItems[X][Y] = 5;
	}

	for (int y = 0; y < config.SERPENT_COUNT; y++) {
		std::pair<int, int> location = getRandomUnoccupiedSpace();
		int X = location.first;
		int Y = location.second;
		config.tiles[X][Y] = 3;
	}
}

std::pair<std::vector<sf::Sprite>, std::vector<sf::Sprite>> Game::generateSpecialItemAndSerpentSprites()
{
	std::vector<sf::Sprite> specialSprites;
	std::vector<sf::Sprite> serpentSprites;

	for (int y = 0; y < config.height; y++) {
		for (int x = 0; x < config.width; x++) {
			if (config.tiles[x][y] == 3) {
				sf::Sprite serpent(getResource("serpent-l", 2));
				serpent.setPosition({x * TILESIZE, y * TILESIZE});
				serpentSprites.push_back(serpent);
			} else if (config.specialItems[x][y] == 1) {
				sf::Sprite machete(getResource("machete", 2));
				machete.setPosition({x * TILESIZE, y * TILESIZE});
				specialSprites.push_back(machete);
			} else if (config.specialItems[x][y] == 2) {
				sf::Sprite flashlight(getResource("flashlight", 2));
				flashlight.setPosition({x * TILESIZE, y * TILESIZE});
				specialSprites.push_back(flashlight);
			} else if (config.specialItems[x][y] == 3) {
				sf::Sprite gold(getResource("gold", 2));
				gold.setPosition({x * TILESIZE, y * TILESIZE});
				specialSprites.push_back(gold);
			} else if (config.specialItems[x][y] == 4) {
				sf::Sprite key(getResource("key", 2));
				key.setPosition({x * TILESIZE, y * TILESIZE});
				specialSprites.push_back(key);
			} else if (config.specialItems[x][y] == 5) {
				sf::Sprite door(getResource("door", 2));
				door.setPosition({x * TILESIZE, y * TILESIZE});
				specialSprites.push_back(door);
			}
		}
	}

	return {specialSprites, serpentSprites};
}

// serpentSprite: which serpent sprite to change
// direction: 0 or 1, left or right
void Game::switchSerpentSprite(sf::Sprite& serpentSprite, int direction)
{
	std::string endingString;
	if (direction == 0)	endingString = "l";
	if (direction == 1)	endingString = "r";
	std::string serpentTexture = "serpent-" + endingString;
	serpentSprite.setTexture(getResource(serpentTexture, 2));
	return;
}

void Game::moveSerpent(sf::Sprite& serpentSprite)
{
	sf::Vector2f serpentPosition = serpentSprite.getPosition();
	// divide by TILESIZE to get actual X and Y tile positions
	int serpentX = serpentPosition.x / TILESIZE;
	int serpentY = serpentPosition.y / TILESIZE;
	if (config.playerX == serpentX && config.playerY == serpentY) {
		// we set this tile to have a serpent in it so that the player doesn't
		// end a move in the same tile as a serpent without anythign happening
		config.tiles[serpentX][serpentY] = 3;
		return;
	}
	config.tiles[serpentX][serpentY] = 0;
	do {
		int direction = straightLineBetweenPoints(config.playerX, config.playerY, serpentX, serpentY);
		if (direction == 0)  direction = directionToMoveForRandomAdjacentSpace(serpentX, serpentY);
		if (direction == 1)  serpentY--;
		if (direction == 2)  serpentY++;
		if (direction == 3) {
			serpentX--;
			switchSerpentSprite(serpentSprite, 0);
		}
		if (direction == 4) {
			serpentX++;
			switchSerpentSprite(serpentSprite, 1);
		}
	} while (hasWall(serpentX, serpentY));
	sf::Vector2f newSerpentPosition(serpentX * TILESIZE, serpentY * TILESIZE);
	serpentSprite.setPosition(newSerpentPosition);
	config.tiles[serpentX][serpentY] = 3;
	return;
}

void Game::moveSerpents()
{
	for (auto& sprite : serpentSprites) {
		moveSerpent(sprite);
	}
}

// 0 = left, 1 = right
// direction, key, machete
void Game::updatePlayerSprite(sf::Sprite& playerSprite)
{
	int key = config.hasKey;
	int machete = config.machetes > 0;
	std::string playerTexture = "player" + std::to_string(config.playerDirection) + std::to_string(key) + std::to_string(machete);
	playerSprite.setTexture(getResource(playerTexture, 2));
	return;
}

// Clear resources. More can be added here later.
void Game::shutDownGame()
{
	groundTextures.clear();
	wallTextures.clear();
	specialTextures.clear();
}

void Game::addAutoGenMazeToTiles()
{
	// Configure mazegen with our preferred options
	mazegen::Config cfg;
	cfg.ROOM_BASE_NUMBER = config.ROOM_BASE_NUMBER;
	cfg.ROOM_SIZE_MIN = config.ROOM_SIZE_MIN;
	cfg.ROOM_SIZE_MAX = config.ROOM_SIZE_MAX;
	cfg.EXTRA_CONNECTION_CHANCE = 0.2;
	cfg.WIGGLE_CHANCE = 0.65;
	cfg.DEADEND_CHANCE = 1.0;
	cfg.RECONNECT_DEADENDS_CHANCE = 1.0;

	mazegen::PointSet constraints {{1, 1}, {config.width - 2, config.height - 2}};

	auto gen = mazegen::Generator();
	gen.generate(config.width, config.height, cfg, constraints);

	if (!gen.get_warnings().empty()) {
		std::cout << gen.get_warnings() << std::endl;
	}

	// long term ideally we would not have two loops like this,
	// but right now it's required so that we can use the right
	// wall sprites
	// plus, this will be useful for custom mazes
	for (int y = 0; y < gen.maze_height(); y++) {
		for (int x = 0; x < gen.maze_width(); x++) {
			int region = gen.region_at(x, y);
			// we only need to check if there's a wall there
			// as everything is ground by default
			if (region == mazegen::NOTHING_ID)  config.tiles[x][y] = 1;
		}
	}
}

std::pair<std::vector<sf::Sprite>, std::vector<sf::Sprite>> Game::generateAndDrawWallsAndGround(int customWidth, int customHeight)
{
	if (groundTextures.empty() || wallTextures.empty()) loadWallsAndGroundResources();

	std::vector<sf::Sprite> groundSpritesToReturn;
	std::vector<sf::Sprite> wallSpritesToReturn;

	bool neededAdjustment = false;
	if (customWidth % 2 == 0) {
		neededAdjustment = true;
		customWidth--;
	}
	if (customHeight % 2 == 0) {
		neededAdjustment = true;
		customHeight--;
	}
	if (neededAdjustment == true) {
		neededAdjustment = false;
		std::cout << "Warning: your custom width and custom height must both be odd! Fixed by subtracting one from both values." << std::endl;
	}

	config.width = customWidth;
	config.height = customHeight;

	if (config.customMaze == false) {
		config.tiles.assign(customWidth, std::vector<int>(customHeight, 0));
		addAutoGenMazeToTiles();
	}

	for (int y = 0; y < customHeight; y++) {
		for (int x = 0; x < customWidth; x++) {
			if (config.tiles[x][y] == 1) {
				std::string wallTexture = "wall";

				// wall textures are named where 0 = flat wall, 1 = protrusion
				// so wall0000 is a standalone pillar, while wall1111 connects
				// on all sides. we use TDLR, so wall1001 will connect top and
				// right but have empty faces pointing left and down. 2 equals
				// emptiness, e.g. outside of the maze, without wall or ground
				wallTexture += std::to_string(texturingWallType(x, y - 1));
				wallTexture += std::to_string(texturingWallType(x, y + 1));
				wallTexture += std::to_string(texturingWallType(x - 1, y));
				wallTexture += std::to_string(texturingWallType(x + 1, y));

				// horizontal and vertical sprites have two forms for a little
				// more variety, so pick a random one to use
				// only applies if it's an internal wall
				if (wallTexture == "wall0011" || wallTexture == "wall1100")  wallTexture += std::to_string(getRandomNumber(0, 1));

				sf::Sprite sprite(getResource(wallTexture, 1));

				sprite.setPosition({x * TILESIZE, y * TILESIZE});

				wallSpritesToReturn.push_back(sprite);
			} else {
				// pick a random ground texture
				const int tex = getRandomNumber(0, 30);
				std::string groundTexture = "";

				if (tex >= 0 && tex <= 4)			groundTexture = "ground0";
				else if (tex >= 5 && tex <= 9)		groundTexture = "ground1";
				else if (tex >= 10 && tex <= 12)	groundTexture = "ground2";
				else if (tex == 13)					groundTexture = "ground3";
				else if (tex == 14)					groundTexture = "ground4";
				else if (tex >= 15 && tex <= 17)	groundTexture = "ground5";
				else if (tex >= 18 && tex <= 20)	groundTexture = "ground6";
				else if (tex >= 21 && tex <= 22)	groundTexture = "ground7";
				else if (tex >= 23 && tex <= 24)	groundTexture = "ground8";
				else if (tex >= 25 && tex <= 26)	groundTexture = "ground9";
				else if (tex >= 27 && tex <= 28)	groundTexture = "ground10";
				else if (tex >= 29 && tex <= 30)	groundTexture = "ground11";

				sf::Sprite sprite(getResource(groundTexture, 0));

				sprite.setPosition({x * TILESIZE, y * TILESIZE});

				groundSpritesToReturn.push_back(sprite);
			}
		}
	}

	return {wallSpritesToReturn, groundSpritesToReturn};
}

void Game::reset()
{
	config.hasKey = config.STARTS_WITH_KEY;
	config.hasLight = config.STARTS_WITH_LIGHT;
	config.machetes = config.MACHETE_STARTING_COUNT;
	config.gold = config.GOLD_STARTING_COUNT;
	config.serpents = config.SERPENT_COUNT;
	config.width = config.DEFAULT_WIDTH;
	config.height = config.DEFAULT_HEIGHT;
	config.playerX = 0;
	config.playerY = 0;
	config.tiles.clear();
	config.specialItems.clear();
	groundSprites.clear();
	serpentSprites.clear();
	specialSprites.clear();
	wallSprites.clear();
}



// pass in backgroundSprites for the game over menu
int Game::InitializeAndRunGame(GlobalData& globalData, GameCfg::Config& gameConfig)
{
	bool returnImmediately = false;
	ShowGameStartMenu(globalData, gameConfig, returnImmediately);
	if (returnImmediately == true)
		return 0;

	config = gameConfig;

	// Set up camera and UI camera
	sf::Vector2f windowSize(static_cast<float>(globalData.window.getSize().x), static_cast<float>(globalData.window.getSize().y));
	sf::View camera(sf::FloatRect({0.f, 0.f}, windowSize));
	sf::View ui(sf::FloatRect({0.f, 0.f}, windowSize));

	// Set up Candle lighting
	candle::RadialLight light;
	light.setRange(config.LIGHT_DEFAULT_SIZE);
	light.setFade(true);
	if (config.hasLight == true)   light.setRange(config.LIGHT_ENLARGED_SIZE);

	float fogWidth = config.width * TILESIZE;
	float fogHeight = config.height * TILESIZE;
	// create the lighting area
	candle::LightingArea fog(candle::LightingArea::FOG,
							 sf::Vector2f(0.0f, 0.0f),
							 sf::Vector2f({fogWidth, fogHeight}));
	fog.setAreaColor(sf::Color::Black);

	// Set up non-player sprites
	if (groundTextures.empty() || wallTextures.empty() || specialTextures.empty())   loadResources();
	std::pair<std::vector<sf::Sprite>, std::vector<sf::Sprite>> wallAndGroundItems = generateAndDrawWallsAndGround(config.width, config.height);
	wallSprites = wallAndGroundItems.first;
	groundSprites = wallAndGroundItems.second;
	if (config.customMaze == false) setUpTileArrays();
	std::pair<std::vector<sf::Sprite>, std::vector<sf::Sprite>> otherSprites = generateSpecialItemAndSerpentSprites();
	specialSprites = otherSprites.first;
	serpentSprites = otherSprites.second;

	// Set up player
	if (config.playerX == 0 && config.playerY == 0) {
		std::pair<int, int> playerLocation = getRandomUnoccupiedSpace();
		config.playerX = playerLocation.first;
		config.playerY = playerLocation.second;
		config.tiles[config.playerX][config.playerY] = 2;
	}
	sf::Sprite player(getResource("player-r", 2));
	player.setPosition({config.playerX * TILESIZE, config.playerY * TILESIZE});
	// make sure they start with the key / machetes they need
	updatePlayerSprite(player);

	// Define how far apart we want UI items
	float uiPadding = 15.f;

	// Set up UI items
	sf::Sprite keyIcon(getResource("key-grey", 2));
	keyIcon.setScale({2.0f, 2.0f});
	keyIcon.setPosition({20.f, 10.f});
	sf::FloatRect keyBounds = keyIcon.getGlobalBounds();
	if (config.hasKey == true) keyIcon.setTexture(getResource("key", 2));

	sf::Vector2f lightPosition{
		keyBounds.position.x + keyBounds.size.x + uiPadding,
		keyBounds.position.y
	};
	sf::Sprite lightIcon(getResource("flashlight", 2));
	lightIcon.setScale({2.0f, 2.0f});
	lightIcon.setPosition(lightPosition);
	sf::FloatRect lightBounds = lightIcon.getGlobalBounds();

	sf::Vector2f goldPosition{
		lightBounds.position.x + lightBounds.size.x + uiPadding,
		lightBounds.position.y
	};
	sf::Sprite goldIcon(getResource("gold", 2));
	goldIcon.setPosition(goldPosition);
	sf::FloatRect goldBounds = goldIcon.getGlobalBounds();

	sf::Vector2f machetePosition{
		goldBounds.position.x + goldBounds.size.x + uiPadding,
		goldBounds.position.y
	};
	sf::Sprite macheteIcon(getResource("machete", 2));
	macheteIcon.setPosition(machetePosition);
	sf::FloatRect macheteBounds = macheteIcon.getGlobalBounds();

	bool keyPressedThisFrame;
	int playerWins = 0;
	bool returnToMainMenu = false;
	bool shouldBreak = false;

	// Start the game loop
	while (globalData.window.isOpen())
	{
		keyPressedThisFrame = false;

		// Reset current player position to be marked as ground.
		config.tiles[config.playerX][config.playerY] = 0;

		while (const std::optional event = globalData.window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				globalData.window.close();
			if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
				sf::Vector2f mousePos = globalData.window.mapPixelToCoords(mousePressed->position, camera);

				int playerX = config.playerX * TILESIZE;
				int playerY = config.playerY * TILESIZE;
				bool greaterThanX = false;
				bool greaterThanY = false;
				bool clickedPlayerX = false;
				bool clickedPlayerY = false;

				if (mousePos.x > playerX + TILESIZE) {
					greaterThanX = true;
				} else if (mousePos.x < playerX) {
					// do nothing: just used for checking if they clicked on player tile
				} else {
					clickedPlayerX = true;
				}
				if (mousePos.y > playerY + TILESIZE) {
					greaterThanY = true;
				} else if (mousePos.y < playerY) {
					// do nothing: just used for checking if they clicked on player tile
				} else {
					clickedPlayerY = true;
				}
				int mouseXComparedToPlayer = std::abs(static_cast<int>(mousePos.x) - playerX);
				int mouseYComparedToPlayer = std::abs(static_cast<int>(mousePos.y) - playerY);

				if (clickedPlayerX && clickedPlayerY) {
					if (getSpecialItem(config.playerX, config.playerY) == 5 && config.hasKey == true) {
						// Player wins!
						playerWins = 1;
						shouldBreak = true;
					}
				} else if (mouseXComparedToPlayer > mouseYComparedToPlayer) {
					if (greaterThanX == false) {
						bool canMove = !hasWall(config.playerX - 1, config.playerY);
						if (canMove) {
							config.playerX--;
							config.playerDirection = 0;
							updatePlayerSprite(player);
						}
						keyPressedThisFrame = true;
					} else {
						bool canMove = !hasWall(config.playerX + 1, config.playerY);
						if (canMove) {
							config.playerX++;
							config.playerDirection = 1;
							updatePlayerSprite(player);
						}
						keyPressedThisFrame = true;
					}
				} else {
					if (greaterThanY == false) {
						bool canMove = !hasWall(config.playerX, config.playerY - 1);
						if (canMove) {
							config.playerY--;
						}
						keyPressedThisFrame = true;
					} else {
						bool canMove = !hasWall(config.playerX, config.playerY + 1);
						if (canMove) {
							config.playerY++;
						}
						keyPressedThisFrame = true;
					}
				}
			}
			if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
				auto code = keyPressed->code;
				using Key = sf::Keyboard::Key;

				if (code == Key::Right || code == Key::D) {
					bool canMove = !hasWall(config.playerX + 1, config.playerY);
					if (canMove) {
						config.playerX++;
						config.playerDirection = 1;
						updatePlayerSprite(player);
					}
					keyPressedThisFrame = true;
				}
				if (code == Key::Left || code == Key::A) {
					bool canMove = !hasWall(config.playerX - 1, config.playerY);
					if (canMove) {
						config.playerX--;
						config.playerDirection = 0;
						updatePlayerSprite(player);
					}
					keyPressedThisFrame = true;
				}
				if (code == Key::Down || code == Key::S) {
					bool canMove = !hasWall(config.playerX, config.playerY + 1);
					if (canMove)   config.playerY++;
					keyPressedThisFrame = true;
				}
				if (code == Key::Up || code == Key::W) {
					bool canMove = !hasWall(config.playerX, config.playerY - 1);
					if (canMove)   config.playerY--;
					keyPressedThisFrame = true;
				}
				if (code == Key::Escape) {
					int gamePauseReturn = ShowGamePauseMenu(globalData);
					if (gamePauseReturn == 1) {
						reset();
						return 1;
					}
					if (gamePauseReturn == 0)   returnToMainMenu = false;
					if (gamePauseReturn == 2)   returnToMainMenu = true;
					shouldBreak = returnToMainMenu;
				}
				if (code == Key::Space || code == Key::Enter) {
					if (getSpecialItem(config.playerX, config.playerY) == 5 && config.hasKey == true) {
						// Player wins!
						playerWins = 1;
						shouldBreak = true;
					}
				}
			}
			if (const auto* windowResized = event->getIf<sf::Event::Resized>()) {
				windowSize = {static_cast<float>(windowResized->size.x), static_cast<float>(windowResized->size.y)};
				ui = sf::View(sf::FloatRect({0.f, 0.f}, windowSize));
				camera = sf::View(sf::FloatRect({0.f, 0.f}, windowSize));
				camera.setCenter({config.playerX * TILESIZE, config.playerY * TILESIZE});
			}
		}

		// calculate X and Y positions of the player, used for camera
		// movement and a bunch of other stuff
		float X = config.playerX * TILESIZE;
		float Y = config.playerY * TILESIZE;
		light.setPosition({X, Y});

		if (keyPressedThisFrame) {
			sf::Vector2f playerPos(X, Y);
			// If the player moved this frame, move them.
			player.setPosition(playerPos);

			moveSerpents();

			// If the player runs over an item, set that they have
			// done so and remove it.
			if (hasSpecialItem(config.playerX, config.playerY)) {
				int item = getSpecialItem(config.playerX, config.playerY);

				if (item == 1) {
					config.machetes++;
					updatePlayerSprite(player);
				}
				if (item == 2) {
					config.hasLight = true;
					light.setRange(config.LIGHT_ENLARGED_SIZE);
				}
				if (item == 3)   config.gold++;
				if (item == 4) {
					config.hasKey = true;
					keyIcon.setTexture(getResource("key", 2));
					updatePlayerSprite(player);
				}

				if (item < 5) {
					config.specialItems[config.playerX][config.playerY] = 0;

					sf::Vector2f targetPos(X, Y);

					specialSprites.erase(
						std::remove_if(specialSprites.begin(), specialSprites.end(), [targetPos](const sf::Sprite& sprite) {
							return sprite.getPosition() == targetPos;
						}),
						specialSprites.end()
					);
				}
			}

			// If the player met a snake, resolve the conflict between them.
			if (snakeInSquare(config.playerX, config.playerY)) {
				if (config.machetes > 0) {
					config.machetes--;
					updatePlayerSprite(player);
					config.serpents--;

					sf::Vector2f targetPos(X, Y);
					serpentSprites.erase(
						std::remove_if(serpentSprites.begin(), serpentSprites.end(), [targetPos](const sf::Sprite& sprite) {
							return sprite.getPosition() == targetPos;
						}),
						serpentSprites.end()
					);
				}
				else {
					playerWins = 0;
					shouldBreak = true;
				}
			}
		}

		if (shouldBreak)	break;

		// Move camera
		sf::Vector2f currentCenter = camera.getCenter();
		sf::Vector2f targetCenter(X, Y);
		sf::Vector2f newCenter = currentCenter + (targetCenter - currentCenter) * 0.05f;
		if (std::abs(targetCenter.x - newCenter.x) < 0.1f && std::abs(targetCenter.y - newCenter.y) < 0.1f)	 newCenter = targetCenter;
		camera.setCenter(newCenter);

		// Mark the player's current position as player-occupied.
		config.tiles[config.playerX][config.playerY] = 2;

		// Candle stuff
		fog.clear();
		fog.draw(light);
		fog.display();

		// Clear screen and use the camera for drawing temporarily
		globalData.window.clear();
		globalData.window.setView(camera);

		// Draw ground, wall, special items, serpents
		for (const auto& ground : groundSprites) {
			globalData.window.draw(ground);
		}

		for (const auto& wall : wallSprites) {
			globalData.window.draw(wall);
		}

		for (const auto& specialSprite : specialSprites) {
			globalData.window.draw(specialSprite);
		}

		for (const auto& serpentSprite : serpentSprites) {
			globalData.window.draw(serpentSprite);
		}

		// Draw player
		globalData.window.draw(player);

		// Draw Candle stuff
		globalData.window.draw(fog);

		// For drawing UI items, we need to make sure
		// they don't move around as the player does,
		// so we have a separate camera for them.
		// window.getDefaultView would solve this,
		// but it doesn't handle resizes properly.
		globalData.window.setView(ui);

		// Draw UI
		// TODO: handle screens that are small enough
		// they can't draw all of the items on one
		// line?
		globalData.window.draw(keyIcon);
		if (config.hasLight)   globalData.window.draw(lightIcon);
		// these braces aren't related to anything but
		// make it easier to visually parse.  all gold
		// sprites are drawn regardless of whether the
		// player has picked them up
		// logic here is used to draw the gold sprites
		// on two lines
		{
			float goldWidth = goldIcon.getGlobalBounds().size.x;
			float goldHeight = goldIcon.getGlobalBounds().size.y;

			goldIcon.setTexture(getResource("gold", 2));

			sf::Vector2f nextGoldPosition{0.0f, 0.0f};

			int goldDrawn = 0;

			// draw gold the player has already grabbed
			for (int i = 0; i < config.gold; i++) {
				nextGoldPosition = {
					goldPosition.x + goldDrawn / 2 * (goldWidth + uiPadding),
					// note: this works, but it does assume there's empty space
					// on the top and bottom of the gold sprite. if that ever gets
					// changed (which I don't anticipate), we may run into trouble here.
					goldPosition.y + goldDrawn % 2 * (goldHeight)
				};
				goldIcon.setPosition(nextGoldPosition);
				globalData.window.draw(goldIcon);
				goldDrawn++;
			}

			goldIcon.setTexture(getResource("gold-grey", 2));

			// draw gold the player can grab somewhere in the maze
			for (int i = 0; i < (config.GOLD_COUNT + config.GOLD_STARTING_COUNT - config.gold); i++) {
				nextGoldPosition = {
					goldPosition.x + goldDrawn / 2 * (goldWidth + uiPadding),
					// see above comment
					goldPosition.y + goldDrawn % 2 * (goldHeight)
				};
				goldIcon.setPosition(nextGoldPosition);
				globalData.window.draw(goldIcon);
				goldDrawn++;
			}

			goldIcon.setPosition(goldPosition);
			goldBounds = goldIcon.getGlobalBounds();
			// calculate where we should start drawing machete sprites
			// this allows us to have a different amount then the default
			// number of gold without worrying about how to put machetes
			// in the right place
			machetePosition = {
				goldBounds.position.x + (config.GOLD_COUNT + config.GOLD_STARTING_COUNT + 1) / 2 * (goldBounds.size.x + uiPadding),
				macheteBounds.position.y
			};
		}
		if (config.machetes > 0) {
			float macheteWidth = macheteIcon.getGlobalBounds().size.x;
			float macheteHeight = macheteIcon.getGlobalBounds().size.y;

			for (int i = 0; i < config.machetes; i++) {
				sf::Vector2f nextMachetePosition{
					machetePosition.x + i / 2 * (macheteWidth + uiPadding),
					// see above comment on the gold
					machetePosition.y + i % 2 * (macheteHeight)
				};
				macheteIcon.setPosition(nextMachetePosition);
				globalData.window.draw(macheteIcon);
			}

			macheteIcon.setPosition(machetePosition);
			macheteBounds = macheteIcon.getGlobalBounds();
		}

		globalData.window.display();
	}

	reset();
	if (returnToMainMenu == false)   ShowGameOverMenu(globalData, playerWins);
	return 0;
}
