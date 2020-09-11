#pragma once

#include <iostream>
#include <GL/freeglut.h>
#include <string>

#include "Common.h"

class World
{
public:

	// constructor
	World();

	// destructor
	~World();
	
	// render the world on a given level
	void drawWorld(int);

	// square size of the world
	int worldSize = 50;

	// boxes in the world
	box boxes[8];
	int totalBoxes = 0;

	// stars in the world
	star stars[8];
	int totalStars = 0;

	// draw a big pillar for debugging
	void drawPillar(float, float);
	void drawWall(float, float, float, float);

	// rotation of the stars (all uniform)
	float starRotation = 0;

	// get the score as a string
	std::string score_toString();

	// get the score as an integer
	int getScore();

	// reset the world
	void reset();

	// draw pillars at the corner of each box
	void outlineBoxes();

private:

	void worldZero();
	void worldOne();
	void worldTwo();
	void worldThree();

	// draw a wire floor
	void drawFloor();


	// draw a solid floor
	void drawCheckeredFloor();
	

	// draw a skybox around the world
	// void drawSkybox();

	// draw a box of a given size at a given position
	void drawBox(int, float, float, float, float, float, float);

	// draw a star at a given position
	void drawStar(int, float, float, float);

};