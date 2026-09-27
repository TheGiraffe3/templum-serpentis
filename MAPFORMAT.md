Maps are in the following format. If the parser discovers a token it doesn't understand, the map will fail to load. Please make sure you get it exact to avoid issues.  
Note that none of the values on the first three lines are optional; even if you want it the same as vanilla you have to explicitly write the value in.  
A few example maze configuration files can be found in the [maps directory](/data/maps/).

Line 1 — a series of numbers defining (in order) collectible machetes, starting machetes, collectible gold, starting gold, whether the player starts with a key (0 if no, 1 if yes), and serpents to add. In vanilla, the default values for a random maze are 2,0,6,0,0,3. If you plan on building your own maze, you can set all values to 0 (unless you'd like the player to start with machetes in which case that value should be a different number)

Line 2 — if there is a flashlight, both varieties of flashlight size in pixels and flashlight by default. Token 1 is a bool value (0 if no, 1 if yes) if a flashlight will be available for the player to pick up. Token 2 is visible distance at the beginning of the game, and token 3 is visible distance after you grab the flashlight (or default if you start with one). Token 4 is whether the player starts with a flashlight (0 if no, 1 if yes). If the player starts with a flashlight, no flashlight will be available for pickup. Vanilla 1,300,600,0.

Line 3 — x and y sizes of the maze (width and height; must both be odd and above or equal to 7 to avoid crashes, ideally less than 100x100 to save RAM and keep the game lightweight), whether to generate a random maze (0 if yes, 1 if no), min room size, max room size, and max number of rooms in the maze. Vanilla 45,45,0,3,7,30. If you don't provide a line 4 and set token 3 to 0, something bad will happen.

Lines 4-end — if you are providing a maze AND if token 3 of line 3 is 0, define the maze here. Put an outer wall around it and make sure all lines are the same number of characters long to avoid undefined behavior. You must have one `P` for the player, one `K` for the key, and one `D` for the door, but other items are optional. Key:  
`#` — wall  
`.` — ground tile  
`&` — gold piece  
`F` — flashlight  
`K` — key  
`S` — serpent  
`C` — machete  
`D` — door  
`P` — player

A maze could look something like this.
```
############
#....###..D#
#C#....#&..#
#.#..#&###.#
#.#..#.....#
#.#..#..S..#
#.F.###....#
#.#.....#..#
#.#.###..&.#
#P#.....####
#.C..S....K#
############
```

Or this.
```
#######
#D...&#
#S#&#S#
#.C&C.#
##&.&##
#K.P.&#
#######
```

Once you've got your maze, you can load it from within the game using the Load Maze screen.  
If you plan to release your maze, playtesting your maze several times is recommended to make sure it actually provides a fun experience.  
If the maze fails to load, run the game binary directly from a terminal and look at the console logs. They should be detailed enough to help you figure out why it isn't working. If not, feel free to open a bug report!

Enjoy!