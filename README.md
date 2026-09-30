## Templum Serpentis

Explore a maze of stone, stone, pinches of gold dust, and... more stone. Find a key to escape, or spend your days wandering and dodging snakes.  
Templum Serpentis is a simple pixel art roguelike built around serpents and machetes - one of which is scattered randomly around the maze, and the other of which move in random directions attempting to find the player. Serpent pathfinding is basic: if they have line of sight to the player, they move in that direction; otherwise, they move in a random direction.  
The easy setting is a "beginner's map" which is relatively hard to lose. The default (medium) provides more of a challenge.

If you find a bug or unexpected behavior, don't hesitate to open an issue or pull request! I'd love to get feedback.

To win a game, find the key and get to the door. If you find a flashlight, you will be able to see further. Gold can be picked up at random spots in the maze, but it doesn't do anything (but can you find every piece of gold before escaping?) If you run into a serpent, you lose - unless you have a machete, in which case the serpent dies.  
Because serpents have relatively basic pathfinding, as mentioned above, it is possible to lure them out of narrow passageways so that you can sneak past. And because of how collisions are handled, you can kill multiple serpents with the same machete. Have fun figuring out how!

Templum Serpentis was created on a Mac, and has not yet been thoroughly tested on Windows or Linux. Pull requests adding full support for (or issues confirming that the game works on) either platform are welcome!



### Controls
Controls cannot be remapped, unless you are willing to edit and build the source. However, I've included what I think are reasonable defaults:  
Up - W or up arrow  
Down - S or down arrow  
Left - A or left arrow  
Right - D or right arrow  
Exit Maze - Space or Enter

Using the mouse is also supported. If you click, the player will move one square in the direction of your pointer. If you click on the space you are currently standing on, you will exit through the door (if you are on the correct square and have a key, that is).

Menu navigation can be done with either the mouse or the keyboard.



### Screenshots + Custom Mazes

![gameplay image 1: small 11x11 map](/img/gameplay1.png)

With custom mazes (see [MAPFORMAT.md](/MAPFORMAT.md) for instructions on how to make one), you can have a map as big or as small as you'd like. You can customize almost everything...

![gameplay image 2: flashlight distance of 100](/img/gameplay2.png)

... even the flashlight size!

![gameplay image 3: 61x61 map](/img/gameplay3.png)

Maps can, theoretically, go as large as your computer supports. (Or 32,767x32,767, whichever is smaller.)  
Due to practical considerations such as not freezing the CPU, however, I recommend that maps stay below 100x100 (which is about where my computer tops out). If you have not loaded a file or chosen a different difficulty setting, the game defaults to a randomly generated 45x45 map.



### Building

Assuming you have cmake installed and a valid C++ compiler defined, you can build Templum Serpentis on MacOS or Windows with these commands. If you use Linux, more steps may be required.
```
cmake -B build
cmake --build build
```

Your first build may take a while. This is because CMake downloads and compiles the source of three other projects Templum Serpentis depends on. As long as you do not delete the build directory, subsequent builds will be faster.


**Modifications**

You cannot turn off music from within the game. I personally don't see that as a problem, but I'm open to changing it.  
Alternatively, you can comment out the `music.play();` line in `main.cpp` and build as described above.

I'm interested in feedback about whether menu clicks would be nice. I have a click sound, but I'm not sure if it's something that adds value. Thoughts and opinions appreciated.

Getting the outer edges of the window to look nice and even at all resolutions proved to be a bit difficult. Because of that, the game window's resolution will always be a multiple of 32x32 pixels (if you resize it to a different multiple, it should jump to the next highest multiple of 32x32, although I haven't gotten this to work on Windows). If you would prefer that to not be the case, you can comment out this section in `MenuFunctions.cpp` and rebuild.

```cpp
	int newWindowSizeX = newWindowSize.x;
	int newWindowSizeY = newWindowSize.y;

	unsigned int windowX = ((newWindowSizeX + 31) / 32) + (((newWindowSizeX + 31) / 32) % 2 == 1);
	unsigned int windowY = ((newWindowSizeY + 31) / 32) + (((newWindowSizeY + 31) / 32) % 2 == 1);

	sf::Vector2u ourNewWindowSize = {windowX * 32, windowY * 32};
	if (newWindowSizeX != windowX * 32 && newWindowSizeY != windowY * 32) {
		globalData.window.setSize(ourNewWindowSize);
	}
```

Again, pull requests are welcome!



### Acknowledgements

Templum Serpentis utilizes a few great open-source libraries:  
[mazegen](https://github.com/aleksandrbazhin/mazegen): Building a maze generation algorithm was (and still is) way beyond my ability as a programmer. Mazegen allowed me to focus on other parts of the game without spending many more hours building the random maze feature (the alternative: having the same maze over and over! _gasp!_).  
[Candle](https://github.com/MiguelMJ/Candle): The game is pretty easy without fog. Candle helped add a layer of complexity without me needing to write OpenGL shaders (which is something I'd like to do at some point, but can't yet).  
[Jersey 15](https://github.com/scfried/soft-type-jersey): Although not actually a code library, it's nice to have FOSS fonts.  
[libnfd](https://github.com/btzy/nativefiledialog-extended): It's also pretty nice to be able to define your own custom mazes. NFD allowed me to worry about how to extract data from the maze file, instead of figuring out a good way to get the maze file's path from the user.  
[SFML](https://github.com/sfml/sfml): And, of course, SFML! Without SFML drawing all the sprites, handling window initialization, sound playing, and so much more, this game couldn't exist at all. Big thanks to the maintainers of and contributors to that project.



### Licensing

Source files are licensed under GPLv3 Copyright © 2026 TheGiraffe3.  
All PNG, ICNS/ICO, and MD files are under the CC-BY-SA-4.0 license Copyright © 2026 TheGiraffe3.  
`data/TemplumSerpentis.wav` is under CC-BY-NC-4.0 Copyright © 2026 TheGiraffe3.  
`data/font.ttf` is under the SIL OPEN FONT license Copyright 2023 The Soft Type Project Authors.  
`include/mazegen.hpp` is under the MIT license Copyright © 2023 Aleksandr Bazhin.  
`include/Candle/*` is under the MIT license Copyright © 2020 Miguel Mejía Jiménez.  
`include/NFD/*` is under the ZLib license Copyright © Bernard Teo.

For the full text of each license above, please see the [OTHERLICENSES.txt](/OTHERLICENSES.txt) file.
