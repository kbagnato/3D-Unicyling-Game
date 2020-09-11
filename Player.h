#pragma once

#include <Gl/Freeglut.h>
#include <iostream>
#include <string>
using namespace std;

class Player {
public:

	// draw the unicycle
	void drawPlayer();

	// draw the unicycle's bounding box
	void drawBoundingBox();

	// move based on the given keys pressed
	void move(bool[]);

	// keep the player within the world limits
	void keepInside(int);

	// get the player's position
	float getPos_x();
	float getPos_y();
	float getPos_z();

	// get the player's pos as a string
	string pos_toString();

	// get the player's vel as a string
	string vel_toString();

	// get the player's front wheel pos as string
	string frontWheel_toString();

	// get the player's back wheel pos as string
	string backWheel_toString();

	// set the player's y vel
	void setVel_y(float);
	
	// position of the front wheel
	float frontWheel_x = 0;
	float frontWheel_y = 0;
	float frontWheel_z = 0;

	// position of the back wheel
	float backWheel_x = 0;
	float backWheel_y = 0;
	float backWheel_z = 0;

	// get the grounded status
	bool getGrounded();
	
	// set the grounded status
	void setGrounded(bool);

	// set the player's Y pos
	void setPos_y(float);	

	// get the player's forward direction
	float getForward();

	// set the player's position to its last
	void revertPos();

	// reset the player's position
	void reset();

private:

	// bounding box - wireframe that wraps around the player
	bool drawBox = true;
	float size_x = 1;
	float size_y = 8;
	float size_z = 4;

	// render the player
	void drawUnicycle();

	// only spin spokes while moving
	bool spokesMoving = true;

	// position
	float pos_x;
	float pos_y;
	float pos_z;

	// velocity
	float vel_x;
	float vel_y;
	float vel_z;

	// for predictions
	float nextPos_x;
	float nextPos_y;
	float nextPos_z;
	float nextTheta;
	float nextThetaRad;

	// last position
	float lastPos_x;
	float lastPos_y;
	float lastPos_z;
	float lastTheta;
	float lastThetaRad;

	// true if player is in contact with ground
	bool grounded;

	// angle to spin the spokes
	float spokesRotation = 0;

	// forward direction (angle from the z-axis in radians)
	float theta = 0;
	float thetaRad;
};