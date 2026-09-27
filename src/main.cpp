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
#include "LoadMazeMenu.h"
#include "MainMenu.h"
#include "MenuFunctions.h"
#include "ResourcePath.hpp"

int main()
{
	Game game;
	GameCfg::Config config;
	GlobalData globalData;

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
	}

	game.shutDownGame();

	return 0;
}
