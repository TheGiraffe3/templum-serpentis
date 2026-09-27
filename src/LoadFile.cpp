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

#include <fstream>
#include <iostream>

#include "GameConfig.h"
#include "LoadFile.h"
#include "ResourcePath.hpp"

std::vector<std::string> splitString(std::string& string, char splitter)
{
	std::vector<std::string> tokens;
	std::size_t start = 0, end = 0;
	while ((end = string.find(splitter, start)) != std::string::npos) {
		tokens.push_back(string.substr(start, end - start));
		start = end + 1;
	}
	tokens.push_back(string.substr(start));
	return tokens;
}

void handleV1ConfigSetting(GameCfg::Config& config, int configOptions[5], int flashlightOptions[4], int mazeOptions[6])
{
	config.MACHETE_COUNT = configOptions[0];
	config.MACHETE_STARTING_COUNT = configOptions[1];
	config.GOLD_COUNT = configOptions[2];
	config.GOLD_STARTING_COUNT = configOptions[3];
	config.STARTS_WITH_KEY = configOptions[4];
	config.SERPENT_COUNT = configOptions[5];

	config.LIGHT_ENABLED = flashlightOptions[0];
	config.LIGHT_DEFAULT_SIZE = flashlightOptions[1];
	config.LIGHT_ENLARGED_SIZE = flashlightOptions[2];
	config.STARTS_WITH_LIGHT = flashlightOptions[3];

	config.tiles.assign(mazeOptions[0], std::vector<int>(mazeOptions[1], 0));
	config.specialItems.assign(mazeOptions[0], std::vector<int>(mazeOptions[1], 0));
	config.width = mazeOptions[0];
	config.height = mazeOptions[1];
	config.customMaze = mazeOptions[2];
	config.ROOM_SIZE_MIN = mazeOptions[3];
	config.ROOM_SIZE_MAX = mazeOptions[4];
	config.ROOM_BASE_NUMBER = mazeOptions[5];

	config.machetes = config.MACHETE_STARTING_COUNT;
	config.gold = config.GOLD_STARTING_COUNT;
	config.hasKey = config.STARTS_WITH_KEY;
	config.hasLight = config.STARTS_WITH_LIGHT;
	config.serpents = config.SERPENT_COUNT;
}

// returns false if there was an error, true if it worked
bool loadDataFile(std::string filePath, GameCfg::Config& config)
{
	int configOptions[6] = {0,0,0,0,0,0};
	int flashlightOptions[4] = {0,0,0,0};
	int mazeOptions[6] =   {0,0,0,0,0,0};

	std::ifstream dataFile(filePath);

	if (!dataFile.is_open()) {
		std::cerr << "Could not open data file!" << std::endl;
		return false;
	}

	// read config options
	std::string currentLine;
	getline(dataFile, currentLine);
	std::vector<std::string> optionsString = splitString(currentLine);
	if (optionsString.size() != 6) {
		std::cerr << "Config options string is the wrong size!" << std::endl;
		return false;
	}
	int currentNumber;
	for (int i = 0; i < 6; i++) {
		currentNumber = std::stoi(optionsString[i]);
		configOptions[i] = currentNumber;
	}

	// read flashlight options
	getline(dataFile, currentLine);
	optionsString = splitString(currentLine);
	if (optionsString.size() != 4) {
		std::cerr << "Flashlight options string is the wrong size!" << std::endl;
		return false;
	}
	for (int i = 0; i < 4; i++) {
		currentNumber = std::stoi(optionsString[i]);
		flashlightOptions[i] = currentNumber;
	}

	// read maze options
	getline(dataFile, currentLine);
	optionsString = splitString(currentLine);
	if (optionsString.size() != 6) {
		std::cerr << "Maze options string is the wrong size!" << std::endl;
		return false;
	}
	for (int i = 0; i < 6; i++) {
		currentNumber = std::stoi(optionsString[i]);
		mazeOptions[i] = currentNumber;
	}

	// use V1 just in case we ever have another config version
	handleV1ConfigSetting(config, configOptions, flashlightOptions, mazeOptions);

	int GOLD_COUNT = 0;
	int MACHETE_COUNT = 0;
	int SERPENT_COUNT = 0;
	bool specifiedKey = false;
	bool specifiedDoor = false;
	bool specifiedPlayer = false;

	if (config.customMaze == true) {
		std::vector<std::string> mazeLines;
		for (int line = 0; line < config.height; line++) {
			getline(dataFile, currentLine);
			mazeLines.push_back(currentLine);
		}
		for (int y = 0; y < config.height; y++) {
			currentLine = mazeLines[y];
			if (currentLine.size() != config.width) {
				std::cerr << "Maze definition line " << (y + 4) << " is the wrong width!" << std::endl;
				if (currentLine.size() > config.width)  return false;
				std::cerr << "Warning: walls will be used instead of the missing characters." << std::endl;
				std::cout << std::endl;
			}
			char currentCharacter;
			for (int x = 0; x < config.width; x++) {
				currentCharacter = currentLine[x];
				if (currentCharacter == '#') {
					config.tiles[x][y] = 1;
				} else if (currentCharacter == '.') {
					config.tiles[x][y] = 0;
				} else if (currentCharacter == '&') {
					config.tiles[x][y] = 0;
					config.specialItems[x][y] = 3;
					GOLD_COUNT++;
				} else if (currentCharacter == 'F') {
					config.tiles[x][y] = 0;
					config.specialItems[x][y] = 2;
				} else if (currentCharacter == 'K') {
					config.tiles[x][y] = 0;
					config.specialItems[x][y] = 4;
					specifiedKey = true;
				} else if (currentCharacter == 'S') {
					config.tiles[x][y] = 3;
					SERPENT_COUNT++;
				} else if (currentCharacter == 'C') {
					config.tiles[x][y] = 0;
					config.specialItems[x][y] = 1;
					MACHETE_COUNT++;
				} else if (currentCharacter == 'D') {
					config.tiles[x][y] = 0;
					config.specialItems[x][y] = 5;
					specifiedDoor = true;
				} else if (currentCharacter == 'P') {
					config.tiles[x][y] = 2;
					config.playerX = x;
					config.playerY = y;
					specifiedPlayer = true;
				} else {
					config.tiles[x][y] = 1;
				}
			}
		}
		config.MACHETE_COUNT = MACHETE_COUNT;
		config.GOLD_COUNT = GOLD_COUNT;
		config.SERPENT_COUNT = SERPENT_COUNT;
		config.serpents = SERPENT_COUNT;

		if (!specifiedPlayer || !specifiedDoor || (!specifiedKey && !config.hasKey)) {
			std::cerr << "Maze neglects to specify one or more of the following: player, door, key" << std::endl;
			std::cout << "Maze loading failed!" << std::endl;
			return false;
		}
	}

	dataFile.close();

	return true;
}
