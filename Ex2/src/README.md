# Game Programming Course Exercise 2

In this project I implemented a horror-corridor while utilizing shaders and post-processing effects

Basic:
------
The basic game has you in a dark corridor with a spotlight. The spotlight is directional and attenuated. The corridor is endless.

Controls:
---------
W - Move forward  
A - Move left  
S - Move backward  
D - Move right  
Mouse - Move flashlight around

(Optional keys):  
B - Toggle bump mapping on/off  
G - Toggle glow on/off  
-/+ - Decrease/Increase ambient lighting in corridor  
0,1,2,3,4 - Change the post-processing convolution kernel (detailed below)  
F - Initiate end sequence

Required implementations:
-------------------------
I implemented the following optional features as specified in the exercise description:
* Flickering light: I implemented a light flickering system that makes the flashlight flicker randomly every few seconds. I think it's pretty scary! If you the 'f' key you might even see some special flickering...
* Walking left/right: Pretty straightforward
* Using mouse for light control: Yessir
* Other kernels: The game includes the following kernels:  
    0) Identity  
    1) Gaussian blur  
    2) Sharpen  
    3) Edge detection  
    4) Emboss
* Glow: Press 'g' to activate a glow around the light from the flashlight. The way the glow works is that first the game tells the corridor to draw only the light part onto a framebuffer. This is saved in a texture. Now the normal PPBuffer is set up and the complete draw is drawn to it's framebuffer. When the PPBuffer renders we pass it the "glow" texture from before and in the PPBuffer shader it blurs that glow then adds on top of the fully rendered scene.

Bonus implementations:
----------------------
In addition to the features requaired by the exercise, I implemented the following because I thought it looked good or felt good:
* When moving forward the camera lumbers from left to right to simulate the motion of walking.
* Full bump-mapping for the walls and ceiling. Can be enabled by pressing 'b'. Is disabld by default because it doesn't look so great in the distance without mip-mapping.
* Jump scare at the end. Without spoiling too much...when you are ready, press 'f' and play a little more to see what happens. There is even a sound effect but it needs to be compiled with a library called irrKlang.

Final note/s:
-------------
* Please note that the floashlights origin position is not at the center of the screen but slightly off to the bottom left. This is to be able to observe cool angles on the walls. The flashlight moves with the camera left and right, but not up and down while walking.

Have fun!
