//  
//  cgp-projects
//
//  Created by HUJI Computer Graphics course staff, 2013.
//  Tweaked by HUJI Computer Games Programming staff 2014

#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

// GLM headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> // for glm::value_ptr

using namespace glm;

#include "World.h"
#include "Controller.h"

#include "ShaderIO.h"
#include "InputManager.h"

#include <iostream>

/** Implementation Definitions */
#define GRID_WIDTH (50)
#define GRID_LENGTH (50)
// For MidPointDisplacement
#define DISPLACEMENT_START_HEIGHT (50.0f)
#define DISPLACEMENT_ROUGHNESS (1.0f)
// For all deformers
#define MILLIS_PER_DEFORMATION (10)

/** Internal Definitions */

#define	WINDOW_WIDTH         (1024) // initial width of the window               //
#define	WINDOW_HEIGHT         (768) // initial height of the window               //
#define	WINDOW_POS_X        (100) // initial X position of the window         //
#define	WINDOW_POS_Y        (100) // initial Y position of the window         //

#define RC_OK                 (0) // Everything went ok                       //
#define RC_INVALID_ARGUMENTS  (1) // Invalid arguments given to the program   //
#define RC_INPUT_ERROR        (2) // Invalid input to the program             //

#define	ARGUMENTS_PROGRAM     (0) // program name position on argv            //
#define	ARGUMENTS_INPUTFILE   (1) // given input file position on argv        //
#define	ARGUMENTS_REQUIRED    (2) // number of required arguments             //

/** Key definitions */

#define KEY_ESC            ('\e') // Key used to terminate the program - ESC  //

/** display callback */
void display(void);

/** window reshape callback  */
void windowResize(int width, int height);

/** keyboardDown callback  */
void keyboardDown(unsigned char key, int x, int y);

/** keyboardUp callback  */
void keyboardUp(unsigned char key, int x, int y);

/** mouse click callback */
void mouse(int button, int state, int x, int y) ;

/** mouse dragging callback  */
void motion(int x, int y) ;

/** timer callback */
void deformationTimer(int value) ;

/** Global variables */

int     g_nFPS = 0, g_nFrames = 0;              // FPS and FPS Counter
int     g_dwLastFPS = 0;                        // Last FPS Check Time
bool    g_startAnimation = false;
bool    g_duringAnimation = false;

// A global variable for our model (a better practice would be to use a singletone that holds all model):

World *_world;
Controller *_controller;

/** main function */
int main(int argc, char* argv[])
{
    std::cout << "Starting ex2..." << std::endl;
	
    // Initialize GLUT
    glutInit(&argc, argv) ;
#ifdef __APPLE__
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_3_2_CORE_PROFILE) ;
#else
    glutInitContextVersion(3, 3);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
#endif
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(WINDOW_POS_X, WINDOW_POS_Y);
    glutCreateWindow("CGP Ex 2");
	
    // Initialize GLEW
    glewExperimental = GL_TRUE;
    int glewStatus = glewInit();
    if (glewStatus != GLEW_OK) {
        std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;
        exit(1);
    }
	
#ifdef __APPLE__
    GLint sync = 1;
    CGLSetParameter(CGLGetCurrentContext(), kCGLCPSwapInterval, &sync);
#endif
		
    // Set callback functions:
    glutDisplayFunc(display) ;
    glutReshapeFunc(windowResize) ;
    glutKeyboardFunc(keyboardDown);
    glutKeyboardUpFunc(keyboardUp);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
//    glutTimerFunc(100, deformationTimer, 0);   // uint millis int value
	
    // Initialize random seed
    srand(time(NULL));
    
    // Set up game
    _world = new World();
    _controller = new Controller(&Camera::Instance(), _world);

    // Set clear color to black:
    glClearColor(0.0, 0.0, 0.0, 0.0);
		
    // Start events/drawing loop
    glutMainLoop();
	
    return 0;
}

int oldTimeSinceStart = 0;
void display(void)
{
    // Clear the screen buffer
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Update the state
    int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
    int deltaTime = timeSinceStart - oldTimeSinceStart;
    oldTimeSinceStart = timeSinceStart;
    
    // Update the game state
    float dt = (float)deltaTime / 100.0f;
    _world->update(dt);
    _controller->update(dt);

    // Tell the world to draw itself
    _world->draw();
	
    // Swap those buffers so someone will actually see the results... //
    glutSwapBuffers();
    
    // Refresh the display
    glutPostRedisplay();
}

// This method is called when the window is resized
void windowResize(int w, int h)
{    
    // set the new viewport //
    glViewport(0, 0, w, h);
    
    // set the perspective of the camera
    Camera::Instance().resize(w, h);
    
    // Refresh the display //
    glutPostRedisplay();
}

/********************************************************************
 * Function  :	keyboard
 * Arguments :	key : the key that was pressed
 *              x   : x value of the current mouse location
 *              y   : y value of the current mouse location
 * Returns   : n/a
 * Throws    : n/a
 *
 * Purpose   : This function handles all the keyboard input from the user.
 *             It supports terminating the application when the KEY_QUIT is pressed.
 *
 \******************************************************************/
void keyboardDown(unsigned char key, int x, int y)
{
    unsigned int lower_key = tolower(key);
    
    if (lower_key == KEY_ESC)
    {
        exit(RC_OK);
    }
    
    InputManager::Instance().handleKeyDown(lower_key, x, y);
}

void keyboardUp(unsigned char key, int x, int y)
{
    unsigned int lower_key = tolower(key);
    
    InputManager::Instance().handleKeyUp(lower_key, x, y);
}

/********************************************************************
 * Function  :   mouse
 * Arguments :   button  : the button that was engaged in some action
 *               state   : the new state of that button
 *               x       : x value of the current mouse location
 *               y       : y value of the current mouse location
 * Returns   :   n/a
 * Throws    :   n/a
 *
 * Purpose   :   This function handles mouse actions.
 *
 \******************************************************************/
void mouse(int button, int state, int x, int y)
{
    if(button == GLUT_LEFT_BUTTON)
    {
		
    }
    else if (button == GLUT_RIGHT_BUTTON)
    {
        
    }
    
    return;
}


/********************************************************************
 * Function  :   motion
 * Arguments :   x   : x value of the current mouse location
 *               y   : y value of the current mouse location
 * Returns   :   n/a
 * Throws    :   n/a
 *
 * Purpose   :   This function handles mouse dragging events.
 *
 \******************************************************************/
void motion(int x, int y)
{
    return;
}

/**
 * Update the deformation every X frames
 */
//static const int deformationInMilli = 50;
//void deformationTimer(int value) {
//    
////    float t = (float)value / (float)framesPerDeformation;
////    if (t > 1) {
////        value = 0;
////        _terrainDeformer->performDeformationStep();
////    }
//    
//    /* Set the timer to be called again in X milli - seconds. */
//    glutTimerFunc(MILLIS_PER_DEFORMATION, deformationTimer, value);   // uint millis int value
//}
