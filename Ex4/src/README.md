# Game Programming Course Exercise 3

In this project I implemented a 3D space shooter with an infinite asteroid field. Inspired by Battlestar Galactice :)

Video here: https://youtu.be/eDRsresMcLE

IMPORTANT NOTE
==============
In order to run this on the University's "aquarium" computers, build normally with 'make', then set the LD_LIBRARY_PATH environment variable like so:
`setenv LD_LIBRARY_PATH ./lib/`

Core functionality:
------------------
* Endless asteroid field implemented using particle system, billboarding and instancing. The ship cannot escape the asteroid field.
* A Spaceraft with WASD/Space tail-following camera control.
* Collision detection with asteroids; Collision counter; Game Over after five collisions; Visual indicator of lives Left.

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

Required implementations:
-------------------------
I implemented the following optional features as specified in the exercise description:
* Skybox (using the wonderful SpaceScape)
* Better ship mesh - The Viper Mk2 from the rebooted Battlestar Galactica television show.
* Sound and music
    - Space battle music from the Battlestar Galactica television show.
    - Dradis ship sound (also from the show)
    - Thruster sounds
    - Explosion sounds
* Better camera control - Spent WAY too long getting this just right. Just search camerafollow in the git log to see all the iterations...
    - Thrust
    - Inertia
    - Camera with inertia
* Explosions in space - Particle system on impact sends fireballs in all directions

Bonus implementations:
----------------------
In addition to the features requaired by the exercise, I implemented the following because I thought it looked good or felt good:
* Particle-system thrusters - When accelerating the thrusters of the ship show a blue fire created using a particle system.
* Opening camera sequence - To really show off the model of the ship. At the start of the game press space to sweep the camera from the front to the back of the ship. If you get impatient just press space again to begin flying.

Final note/s:
-------------
My true achievement during this exercise was creating the particle system. I think it is robust and efficient. It uses a structure of arrays to hold the particle attributes. This is in sync with how OpenGL holds the dividor VBO's for instancing. This way there is no need to recreate the arrays of attributes every frame but instead they can be accessed and then dumped directly to the GPU.

The ParticleSystem base class handles these particle attributes and rendering etc. and allows deriving classes to simply implement the emit() and updateParticle(particleID, dt) functions. That's all! This allows for all sorts of different kinds of particle systems, as exemplified in the exercise. The asteroids are a system that doesn't kill or generate any new particles, while the explosions create particles ever random amount of time. The thrusters create half-circle shaped fireballs coming out of the ship.

In short, I think it's wonderful and I hope you like it too :)

Dependencies:
-------------
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


I think that's everything. Have fun!
