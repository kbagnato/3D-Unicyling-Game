#pragma once

#include "Game.h"
#include "Common.h"

/**/
/*
Game::Game() Game::Game()

NAME

	Game::Game - constructor for the Game object

SYNOPSIS

	Game::Game();

DESCRIPTION

	This function will create a new Game object and initalize GLUT without
	any specific paramters. It will also call another function of Game
	to initalize the viewing window. It also defines the first level to load.

RETURNS

	No return value.

AUTHOR

	Kevin Bagnato
*/
/**/
Game::Game()
{

	// initalize glut library
	int argc = 1;
	char* argv[1] = { (char*)"Pretend arguments" };
	glutInit(&argc, argv);

	// initialize game window
	initWindow();

	// game state
	level = 2;
}


/**/
/*
Game::~Game() Game::~Game()

NAME

	Game::~Game - destructor for the Game object

SYNOPSIS

	Game::~Game();

DESCRIPTION

	This function currently does nothing.

RETURNS

	No return value.

AUTHOR

	Kevin Bagnato
*/
/**/
Game::~Game()
{
}


/**/
/*
Game::initWindow() Game::initWindow()

NAME

	void Game::initWindow - initalizes the Game's window

SYNOPSIS

	Game::initWindow();

DESCRIPTION

	This function sets the display mode to have a double buffered window,
	with an RGBA color model, and have depth. It will set the window size
	and position it to the center of the user's screen. Finally it will
	open the window.

RETURNS

	No return value.

AUTHOR

	Kevin Bagnato
*/
/**/
void Game::initWindow()
{
	// set display buffer a double depth buffer
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	// set window size
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	// set window position to center of screen
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - WINDOW_WIDTH) / 2, (glutGet(GLUT_SCREEN_HEIGHT) - WINDOW_HEIGHT) / 2);

	// open screen window
	glutCreateWindow(title.c_str());
}


/**/
/*
Game::display() Game::display()

NAME

	void Game::display - render the game's display to the window

SYNOPSIS

	Game::display();

DESCRIPTION

	This function is repeatedly called to create the animation of the game.
	It creates the light in its place. It updates the position of the camera
	and player based on the keys pressed at that time. It will check for box
	collisions as well as star collisions. It will prevent the player from
	moving outside of the world. The function then draws the player and the world.
	It also listens for a level change or reset (manually inputted from the user).
	It provides the 'head-up display' of information for the user. FInally,
	the function renders the display to the window and calls itself again via
	glutPostRedisplay().

RETURNS

	No return value.

AUTHOR

	Kevin Bagnato
*/
/**/
void Game::display()
{
	/*
		determine next player pos
		check world collisions, update player pos
		check star collisions, update score
		draw stats, score
	*/

	// light position
	float light_pos[4] = { 0., 10., 10., 1. };
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

	// update camera position
	c.move(k.keysDown);

	// update player position
	p.move(k.keysDown);

	// check for collisions
	checkStarCollisions();
	checkBoxCollisions();
	checkFloorCollisions();

	// keep player inside world
	p.keepInside(w.worldSize);

	// reset world with r key
	if (k.keysDown['r'])
	{
		resetLevel();
	}

	// change world with number keys
	if (k.keysDown['0'])
	{
		level = 0;
		resetLevel();
	}
	else if (k.keysDown['1'])
	{
		level = 1;
		resetLevel();
	}
	else if (k.keysDown['2'])
	{
		level = 2;
		resetLevel();
	}
	else if (k.keysDown['3'])
	{
		level = 3;
		resetLevel();
	}

	// draw world
	w.drawWorld(level);

	// draw unicycle
	p.drawPlayer();
	
	if (k.keysDown['t'])
	{
		toggleDebug();
	}

	// print stats on screen
	if (debug)
	{
		debugStats();
	}

	displayScore();

	// display when all coins are collected
	if (w.getScore() == w.totalStars)
	{
		drawString("All stars collected! Press Y to continue", 5, 0);

		if (k.keysDown['y'])
		{
			level++;

		}
	}

	// render to screen
	glutPostRedisplay();

	// swap buffers to execute rendering to the screen
	glutSwapBuffers();
}


/**/
/*
Game::checkStarCollisions()Game::checkStarCollisions()

NAME

	void Game::checkStarCollisions - check if the player is colliding with a star

SYNOPSIS

	Game::checkStarCollisions();

DESCRIPTION

	This function will create a temporary bounding box for each star
	in the world and check if the player is in this aforementioned box.
	If s, it hides the given star.

RETURNS

	No return value.

AUTHOR

	Kevin Bagnato
*/
/**/
void Game::checkStarCollisions()
{
	for (int i = 0; i <= w.totalStars; i++)
	{
		box tempStar;
		tempStar.min_x = w.stars[i].pos_x - 1;
		tempStar.max_x = w.stars[i].pos_x + 1;

		tempStar.min_y = w.stars[i].pos_y - 1;
		tempStar.max_y = w.stars[i].pos_z + 1;

		tempStar.min_z = w.stars[i].pos_z - 1;
		tempStar.max_z = w.stars[i].pos_z + 1;

		if (debug && w.stars[i].show)
		{
			cout << "star " << i<< ": " << tempStar.min_x << " - " << tempStar.max_x << endl;
		}
		
		
		if (isPointInBox(p.getPos_x(), p.getPos_y(), p.getPos_z(), tempStar))
		{
			w.stars[i].show = false;
		}
	}
}


/**/
/*
Game::checkBoxCollisions() Game::checkBoxCollisions()

NAME

	void Game::checkBoxCollisions - check if the player is colliding with a box

SYNOPSIS

	Game::checkBoxCollisions();

DESCRIPTION

	This function will loop through all of the world's boxes.
	First create three boolean values: onefor the front of
	the unicycle's wheel, one for the middle, and one for the
	rear. If at least one of these collide with a box, then the following
	will take place: if the top of the box is less than or equal
	to 2 units, then the player will be moved to the top of the box;
	else, the player will be pushed back to effectively keep it out
	of the box.

RETURNS

	No return value.

AUTHOR

	Kevin Bagnato
*/
/**/
void Game::checkBoxCollisions()
{
	for (int i = 0; i <= w.totalBoxes; i++)
	{
		bool front = isPointInBox(p.frontWheel_x, p.frontWheel_y, p.frontWheel_z, w.boxes[i]);
		bool center = isPointInBox(p.getPos_x(), p.getPos_y(), p.getPos_z(), w.boxes[i]);
		bool back = isPointInBox(p.backWheel_x, p.backWheel_y, p.backWheel_z, w.boxes[i]);

		// display 
		if (debug)
		{
			w.drawPillar(p.frontWheel_x, p.frontWheel_z);
			w.drawPillar(p.backWheel_x, p.backWheel_z);
			w.outlineBoxes();

			// multiple boxes so all results overlap
			if (front)
			{
				drawString("front in box " + to_string(i), 6, 0);
			}
			if (center)
			{
				drawString("center: " + to_string(i), 7, 0);
			}
			if (back)
			{
				drawString("back: " + to_string(i), 8, 0);
			}
		}

		// if wheel is in box
		if (front || center || back)
		{
			// if box is low, step onto it
			if (w.boxes[i].max_y - p.getPos_y() <= 2)
			{
				p.setPos_y(w.boxes[i].max_y);
				p.setGrounded(true);
			}
			// if box is high, prevent intersection
			else
			{
				p.setGrounded(false);
				p.revertPos();
			}
		}
	}
}


/**/
/*
Game::checkFloorCollisions() Game::checkFloorCollisions()

NAME

	void Game::checkFloorCollisions - check if the player is colliding with the floor

SYNOPSIS

	Game::checkFloorCollisions();

DESCRIPTION

	This function will determine if the player is on the ground.
	If so, it will prevent the player from falling through.

RETURNS

	No return value.

AUTHOR

	Kevin Bagnato
*/
/**/
void Game::checkFloorCollisions()
{
	if (p.getPos_y() <= 0)
	{
		p.setPos_y(0);
		p.setGrounded(true);
	}
}


/**/
/*
Game::isPointInBox() Game::isPointInBox()

NAME

	Game::isPointInBox - determine if the given point collides with the given box

SYNOPSIS

	Game::isPointInBox(float x, float y, float z, box b);
		float x		->	the X coordinate of the point to check
		float y		->	the Y coordinate of the point to check
		float z		->	the Z coordiante of the point to check
		box b		->	the box to check

DESCRIPTION

	This function will determine if the given point is inside the
	given box, edges inclusive.

RETURNS

	This function will return true if the point is in the box.
	It will return false if the point is not in the box.

AUTHOR

	Kevin Bagnato
*/
/**/
bool Game::isPointInBox(float x, float y, float z, box b)
{
	if (x >= b.min_x && x <= b.max_x &&
		z >= b.min_y && y <= b.max_y &&
		z >= b.min_z && z <= b.max_z)
	{
		return true;
	}
	else
	{
		return false;
	}
}


/**/
/*
Game::drawString() Game::drawString()

NAME

	void Game::drawString - draw a given string on the screen at a given position

SYNOPSIS

	Game::drawString(string s, int line, float x);
		string s		->	the string to print
		int line		->	the line number to print
		float x			->	the X position to print

DESCRIPTION

	This function will draw a given string on the screen. Line numbers start
	at 0, which is the top of the screen. Consectuive integers draw on
	the lines below. The string begins at position 'x'.

RETURNS

	No return value.

AUTHOR

	Kevin Bagnato
*/
/**/
void Game::drawString(string s, int line, float x)
{
	// save projection matrix
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	// save modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// set text color to white
	glColor3f(0, 0, 0);

	// set text position
	glRasterPos2f(x, (float)(1 - line * 0.05));

	// draw text on screen
	for (unsigned int i = 0; i < s.size(); i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, s[i]);
	}

	// restore old projection matrix
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	// restore old modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}


/**/
/*
Game::displayScore() Game::displayScore()

NAME

	void Game::displayScore - display the player's score to the screen

SYNOPSIS

	Game::displayScore();

DESCRIPTION

	This function will display the player's score (the number of stars collected)
	in the top-right corner of the screen.

RETURNS

	No return value.

AUTHOR

	Kevin Bagnato
*/
/**/
void Game::displayScore()
{
	drawString(w.score_toString(), 1, 0);
}


/**/
/*
Game::debugStats() Game::debugStats()

NAME

	void Game::debugStats - display statistics to the screen for debugging

SYNOPSIS

	Game::debugStats();

DESCRIPTION

	This function will display the player's position, velocity,
	grounded status, and wheel intersection status in successive lines
	from the top-left corner of the window.

RETURNS

	No return value.

AUTHOR

	Kevin Bagnato
*/
/**/
void Game::debugStats()
{
	// line 0
	drawString(p.pos_toString(), 0, -1.0);

	// line 1
	drawString(p.vel_toString(), 1, -1.0);

	//line 2
	drawString("grounded: " + to_string(p.getGrounded()), 2, -1.0);

	// line 3
	drawString("fps: " + to_string(fps), 3, -1.0);

}


/**/
/*
Game::resetLevel() Game::resetLevel()

NAME

	void Game::resetLevel - reset the level

SYNOPSIS

	Game::resetLevel();

DESCRIPTION

	This function will call the Player's reset funciton,
	the World's reset function, and the camera's reset function.

RETURNS

	No return value.

AUTHOR

	Kevin Bagnato
*/
/**/
void Game::resetLevel()
{
	p.reset();
	w.reset();
	c.reset();
}


/**/
/*
Game::toggleDebug() Game::toggleDebug()

NAME

	Game::toggleDebug - toggle debugging mode

SYNOPSIS

	Game::toggleDebug();

DESCRIPTION

	This function will invert the boolean value 'debug'.

RETURNS

	No return value.

AUTHOR

	Kevin Bagnato
*/
/**/
void Game::toggleDebug()
{
	debug = !debug;
}