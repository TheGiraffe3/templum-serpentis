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

#include <vector>

namespace GameCfg {
	struct Config {
		// should be kept odd
		static constexpr int DEFAULT_HEIGHT = 45;
		static constexpr int DEFAULT_WIDTH = 45;

		int MACHETE_COUNT = 2;
		int MACHETE_STARTING_COUNT = 0;
		int GOLD_COUNT = 6;
		int GOLD_STARTING_COUNT = 0;
		bool STARTS_WITH_KEY = false;
		int SERPENT_COUNT = 3;

		bool LIGHT_ENABLED = true;
		int LIGHT_DEFAULT_SIZE = 300;
		int LIGHT_ENLARGED_SIZE = 600;
		bool STARTS_WITH_LIGHT = false;

		int width = 45;
		int height = 45;
		bool customMaze = false;
		int ROOM_SIZE_MIN = 3;
		int ROOM_SIZE_MAX = 7;
		int ROOM_BASE_NUMBER = 30;

		// 0 = ground, 1 = wall, 2 = player, 3 = serpent
		std::vector<std::vector<int>> tiles{DEFAULT_WIDTH, std::vector<int>(DEFAULT_HEIGHT, 0)};
		// 0 = none, 1 = machete, 2 = flashlight, 3 = gold, 4 = key, 5 = door
		std::vector<std::vector<int>> specialItems{DEFAULT_WIDTH, std::vector<int>(DEFAULT_HEIGHT, 0)};

		// current game config
		int machetes = MACHETE_STARTING_COUNT;
		int gold = GOLD_STARTING_COUNT;
		int serpents = SERPENT_COUNT;
		bool hasKey = STARTS_WITH_KEY;
		bool hasLight = STARTS_WITH_LIGHT;

		int playerX = 0;
		int playerY = 0;
		int playerDirection = 1;
	};
}
