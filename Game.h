#pragma once

#include <iostream>

#include "Player.h"
#include "Camera.h"
#include "World.h"
#include "Keyboard.h"

using namespace std;

class Game
{
public:

	Game();
	~Game();

	// game definitions
	const string title = "Unkown Unicycler ALPHA 1.1";
	const int WINDOW_WIDTH = 1000;
	const int WINDOW_HEIGHT = 655;
	int fps = 60;
	
	// game objects
	Keyboard k;
	Player p;

	// render buffer to screen
	void display();

private:

	// initalize game window
	void initWindow();

	// game objects
	World w;
	Camera c;
	int level = 0;

	// debugging status
	bool debug = false;

	// handle intersections/collisions
	void checkStarCollisions();
	void checkBoxCollisions();
	void checkFloorCollisions();
	bool isPointInBox(float, float, float, box);

	// draw text on screen
	void drawString(string, int, float);
	void displayScore();
	void debugStats();

	// reset player, world, camera
	void resetLevel();

	// toggle debugging status
	void toggleDebug();
};