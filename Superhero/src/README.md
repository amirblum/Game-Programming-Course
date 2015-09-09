# Game Programming Course Final Project

IMPORTANT NOTE
==============
In order to run this on the University's "aquarium" computers, build normally with 'make', then set the LD_LIBRARY_PATH environment variable like so:
`setenv LD_LIBRARY_PATH ./lib/`
Then run the game with `cgp_superhero`

Intro:
------
This is the final project for the course. I implemented a "superhero" game (the superhero is Buzz Lightyear) flying through a city. The main draw of the game is the boost mechanic where you press and hold space to charge a boost, then unleash a great speed!

Core functionality:
------------------
1. Endless city - you can fly through the city endlessly in any direction
2. Tight controls - the controls are very tight. When turning at high speeds buzz will slow down, much like a race car drifting, so that you can make the turn.
3. Incredible boost mode - this mechanic feels really good. While the space bar is held down time slows down and a dolly zoom takes effect. This time allows the player to align buzz in the direction he wants to boost. When released buzz shoots forward at great speed, while the world blurs around him! The longer you hold down space the faster he will boost.

Controls:
---------
W - Forward
S - Backward  
A - Turn left  
D - Turn right  
Space - Boost!

(Optional keys):  
r - Reset game

Things missing
--------------
There are two things I would add if I had more time:

1. The city has a...very short visibility. This is because I load all the geometry of every building in the grid. With more time I would implement the buildings using a particle system like the astroids, and add a "fog" effect where the buildings fade in so they don't pop in like they do now.
2. A win mechanic. Right now you can technically lose by bumping into too many buildings, but there is nothing to save in the city as originally planned. I would add things to collect like coins or other toys to save.

Final note/s:
-------------
The most important aspect for me was the boost mechanic and I think it feels very good. I think it's unfortunate that the city pops in so close but it at least gives the feeling of the boost as I intended.
Of course, if Mark wants to display it at Hansen I could continue working on it before the display, but this is the version that should be graded.


Dependencies (same as Ex3/4. Repeated here for prosperity):
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
