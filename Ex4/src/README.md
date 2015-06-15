# Game Programming Course Exercise 4

This project builds on the previous exercise, adding more advanced physics with black holes and collisions and stuff!

IMPORTANT NOTE
==============
In order to run this on the University's "aquarium" computers, build normally with 'make', then set the LD_LIBRARY_PATH environment variable like so:
`setenv LD_LIBRARY_PATH ./lib/`
Then run the game with `cgp_ex4`

Core functionality:
------------------
1. Rigid-body physics engine for position. All objects (Ship, asteroids, black holes) have size and mass. The black holes act as static force sources (the rest of the objects in the simulation don't apply force to each other).
2. Ship thrust. Thrusters apply force in the direction of the ship. This force does not move with the direction of the ship as in the previous exercise (if you want to stop/turn around you have to turn the ship around and apply force in the opposite direction)
3. Black holes. As stated black holes apply gravity to the ship and asteroids, inversely proportional to the square distance. Black holes themselves are static.
4. A static space beacon. The beacon takes the form of the Battlestar Galactica. You must reach it!
5. A shield around the ship (so as to show it's radius).

Controls:
---------
W - Tilt down  
S - Tilt up  
A - Roll left  
D - Roll right  
Space - Accelerate

(Optional keys):  
g - Toggle game over  
r - Reset game

Bonus implementations:
-------------------------
I implemented the following optional features as specified in the exercise description:
* Destroy the asteroid if impact is above certain threshhold (1pt)
* A runge-kutta 4 ODE solver (2pts)
* Show thrust using particle system (2pts)
* Beacon compass that shows direction to the beacon at any orientation (1pt) [Thanks to Zeev Adelman for help with the shader on this!]
* Nicer shield. Sphere with semitransparent texture + time based color changes (2pts)
* Music and sound effects (1pt)

Total: 13 (base) + 9 (Optional) = 21 pts

Final note/s:
-------------
This wasn't as big as a jump as exercise 3, but I liked how it came out anyway so nyeh.

Dependencies (same as Ex3. Repeated here for prosperity):
---------------------------------------------------------
This project has three main external library dependencies which I will outline here:

1. Alut.

   The alut project is used for loading .wav files. Alut isn't always installed with OpenAL anymore which is why I consider it an external library. The libalut.so.0 dynamic library is installed on the aquarium computers, but is not linked to by libalut.so, so that's why it is hardcoded in the Makefile. In addition, the header is not in the aquarium's include/ directory, so I have included it in the local project's include/ directory.

2. SOIL2.

   This is a port of the Simple OpenGL Image Library (SOIL), which updates a bunch of features. It is hosted here: https://bitbucket.org/SpartanJ/soil2 . I am actually using a slightly modified build. On both my machine and the aquarium machine, SOIL2 reported that cubemap textures were not supported and refused to load them. I modified the SOIL2.c file to remove that check. You may or may not have to do this on a windows machine. In any case, the modified libsoil.a is in the lib/ directory. Just in case, here is the diff:
   
    ```
    Amirs-MacBook-Pro-2:SOIL2 amirblum$ diff SOIL2.c.ORIG SOIL2.c
    605,609c605,609
    < 	if( query_cubemap_capability() != SOIL_CAPABILITY_PRESENT )
    < 	{
    < 		result_string_pointer = "No cube map capability present";
    < 		return 0;
    < 	}
    ---
    > //	if( query_cubemap_capability() != SOIL_CAPABILITY_PRESENT )
    > //	{
    > //		result_string_pointer = "No cube map capability present";
    > //		return 0;
    > //	}
    ```
    
3. Assimp (The Open Asset Import Library)

   This is a big one. It handles the mesh loading. It can be downloaded from http://assimp.sourceforge.net . Should be a pretty straightforward install. Note that the version for running on aquarium computers, included with this project, is a dynamic library (.so and not .a). Since I don't have permissions to install it on aquarium computers, it will sit in the local ./lib/ folder. When running the project we do need to point it to this folder, which is why I've specified setting LD_LIBRARY_PATH to ./lib/, as detailed above.


Have fun!
