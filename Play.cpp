#pragma once

#include <GL/freeglut.h>
#include <GL/glut.h>
#include <string>
#include <iostream>
#include <ctime>
#include <windows.h>

#include "Player.h"
#include "World.h"
#include "Camera.h"
#include "Keyboard.h"
#include "Game.h"

using namespace std;

// constant variables
const float ASPECT_RATIO = 1.56;

Game g;

// timing variables to limit FPS
time_t timeStart, timeEnd;
clock_t clockStart, clockEnd;

/**/
/*
initalize() initalize()

NAME

	initalize() - initalize paramters

SYNOPSIS

	initalize();

DESCRIPTION

	This function will initalize the windows's background color,
	enable OpenGL lighting and one light, initalize view variables,
	and the camera's frustrum (the portion of the world that the
	camera can see).

RETURNS

	No return value.

AUTHOR

	Kevin Bagnato
*/
/**/
void initalize()
{
	// background color when buffers are cleared
	glClearColor(0.0, 0.6, 1.0, 1.0);

	// enale lighting, light 0
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);

	// not too sure after here?
	glEnable(GL_NORMALIZE);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// set up camera
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-5.0, 5.0, -5.0, 5.0, 10.0, 1000.0);
}


/**/
/*
displayWrapper() displayWrapper()

NAME

	displayWrapper - a wrapper function for OpenGL's display callback function

SYNOPSIS

	glutDisplayFunc(displayWrapper);

DESCRIPTION

	This function is an event handler for special key presses. It will call
	Game's display function and	establish an upper-limit of frames to display per second.

RETURNS

	No return value.

AUTHOR

	Kevin Bagnato
*/
/**/
void displayWrapper()
{
	// start timer
	clockStart = clock();

	// render game scene
	g.display();

	// end timer
	clockEnd = clock();

	// wait to regulate upper FPS limit
	Sleep(1000 / g.fps - ((double)(clockEnd - (double) clockStart) / CLOCKS_PER_SEC));
}


/**/
/*
keyboardDownWrapper() keyboardDownWrapper()

NAME

	keyboardDownWrapper - a wrapper function for OpenGL's keyboard down callback function

SYNOPSIS

	glutKeyboardFunc(keyboardDownWrapper);

DESCRIPTION

	This function is an event handler for special key presses. It will call
	Keyboard's keyboardDown function and listen for the toggle key for debugging mode.
		unsigned char key		->	the key pressed down
		int x					->	the X position of the mouse
		int y					->	the Y position of the mouse

RETURNS

	No return value.

AUTHOR

	Kevin Bagnato
*/
/**/
void keyboardDownWrapper(unsigned char key, int x, int y)
{
	g.k.keyboardDown(key);
}

/**/
/*
keyboardUpWrapper() keyboardUpWrapper()

NAME

	keyboardUpWrapper - a wrapper function for OpenGL's keyboard release callback function

SYNOPSIS

	glutKeyboardUpFunc(keyboardUpWrapper);

DESCRIPTION

	This function is an event handler for special key presses. It will call
	Keyboard's keyboardDown function.
		unsigned char key		->	the key released
		int x					->	the X position of the mouse
		int y					->	the Y position of the mouse

RETURNS

	No return value.

AUTHOR

	Kevin Bagnato
*/
/**/
void keyboardUpWrapper(unsigned char key, int x, int y)
{
	g.k.keyboardUp(key);
}


/**/
/*
specialWrapper() specialWrapper()

NAME

	specialWrapper - a wrapper function for OpenGL's special keyboard press callback function

SYNOPSIS

	glutSpecialFunc(specialWrapper);

DESCRIPTION

	This function is an event handler for special key presses. It will 
	call Keyboard's specialKeys function and listen for	FPS modifier keys.
		unsigned char key		->	the key released
		int x					->	the X position of the mouse
		int y					->	the Y position of the mouse

	Special keys include all function keys (F1, F2, F3, ... F12),
	arrow keys, Page Up, Page Down, Home, End, and Insert.

RETURNS

	No return value.

AUTHOR

	Kevin Bagnato
*/
/**/
void specialWrapper(int key, int x, int y)
{
	g.k.specialKeys(key);

	if (key == GLUT_KEY_UP)
	{
		g.fps++;
	}
	if (key == GLUT_KEY_DOWN)
	{
		g.fps--;
	}
}


/**/
/*
reshape() reshape()

NAME

	reshape - handle window size reshape events

SYNOPSIS

	glutReshapeFunc(reshape);

DESCRIPTION

	This function is an event handler for window size changes. It ensures 
	the pre-defined aspect of the game is maintained.

RETURNS

	No return value.

AUTHOR

	Kevin Bagnato
*/
/**/
void reshape(int width, int height)
{
	float aspectCur = float(width) / (float)height;
	if (aspectCur > ASPECT_RATIO)
	{
		glViewport(0, 0, height*ASPECT_RATIO, height);
	}
	else
	{
		glViewport(0, 0, width, width / ASPECT_RATIO);
	}

	cout << "width: " << width << " height: " << height << endl;
}

/**/
/*
main() main()

NAME

	main - the entry point of the program

SYNOPSIS

	glutReshapeFunc(reshape);

DESCRIPTION

	This function initalizes the paramters of the game, registers
	callback functions for OpenGL, and starts the game.

RETURNS

	No return value.

AUTHOR

	Kevin Bagnato
*/
/**/
int main(int argc, char **argv)
{
	initalize();

	// register callback functions
	glutDisplayFunc(displayWrapper);
	glutKeyboardFunc(keyboardDownWrapper);
	glutKeyboardUpFunc(keyboardUpWrapper);
	glutSpecialFunc(specialWrapper);
	glutReshapeFunc(reshape);

	// begin loop, no code after here will execute
	glutMainLoop();

	return 0;
}