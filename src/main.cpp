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

#include "Game.h"
#include "GameConfig.h"
#include "GlobalData.h"
#include "LoadFile.h"
#include "LoadMazeMenu.h"
#include "MainMenu.h"
#include "MenuFunctions.h"
#include "ResourcePath.hpp"

void PrintHelp()
{
	std::cout << "" << std::endl;
	std::cout << "List of command line options:" << std::endl;
	std::cout << "      -h, --help: print this help message." << std::endl;
	std::cout << "      -m, --map <file>: load a custom map at game startup." << std::endl;
	std::cout << "      -v, --version: print version." << std::endl;
	std::cout << "" << std::endl;
}

int main(int argc, char *argv[])
{
	Game game;
	GameCfg::Config config;
	GlobalData globalData;

	for(const char *const *it = argv + 1; *it; ++it)
	{
		std::string arg = *it;
		if (arg == "-h" || arg == "--help")
		{
			PrintHelp();
			std::cout << "Use the arrow keys or WASD to move. Press Space to enter the door, if you have the key." << std::endl;
			std::cout << "You can also use the mouse; simply click in the direction you want to go, or on your tile to go through the door." << std::endl;
			std::cout << "" << std::endl;
			std::cout << "Please report any bugs to https://github.com/TheGiraffe3/templum-serpentis." << std::endl;
			std::cout << "" << std::endl;
			return 0;
		}
		else if (arg == "-m" || arg == "--map")
		{
			if (*++it)
				loadDataFile(*it, config);
			else
			{
				std::cout << "" << std::endl;
				std::cout << "You must provide a map file to use this config option." << std::endl;
				std::cout << "" << std::endl;
				return 1;
			}
		}
		else if(arg == "-v" || arg == "--version")
		{
			std::cout << "" << std::endl;
			std::cout << "Templum Serpentis v0.1.1" << std::endl;
			std::cout << "https://github.com/TheGiraffe3/templum-serpentis" << std::endl;
			std::cout << "" << std::endl;
			return 0;
		}
		else if(arg != "")
		{
			std::cout << "" << std::endl;
			std::cout << "Unrecognized argument." << std::endl;
			PrintHelp();
			return 1;
		}
	}

	globalData.game = game;

	sf::VideoMode mode = sf::VideoMode::getDesktopMode();
	globalData.window.create(mode, "Templum Serpentis");
	globalData.window.setFramerateLimit(180);

#if defined(_WIN32)
	sf::Image icon;
	if (icon.loadFromFile(getResourcesPath() + "window-icon.png")) {
		globalData.window.setIcon(icon);
	}
#endif

	GenerateBackgroundSprites(globalData);

	std::string path = getResourcesPath();
	sf::Music music(path + "TemplumSerpentis.wav");
	music.setLooping(true);
	music.setVolume(20.0f);
	music.play();

	int toDoNext = 0;

	while (globalData.window.isOpen())
	{
		if (toDoNext == 0) {
			toDoNext = ShowMainMenu(globalData);
		}
		if (toDoNext == 1) {
			toDoNext = 0;
			toDoNext = game.InitializeAndRunGame(globalData, config);
		}
		if (toDoNext == 2) {
			toDoNext = 0;
			ShowLoadMazeMenu(globalData, config);
		}
		if (toDoNext == 3) {
			toDoNext = 0;
			globalData.window.close();
		}
		if (toDoNext == 4) {
			toDoNext = 0;
			toDoNext = game.InitializeAndRunGame(globalData, config, true);
		}
	}

	game.shutDownGame();

	return 0;
}
