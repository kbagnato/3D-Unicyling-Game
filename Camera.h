#pragma once

#include <GL\freeglut.h>
#include <cmath>

class Camera
{
public:
	// move the camera with keyboard
	void move(bool[]);

	void reset();

private:
	// camera position
	float eye_x = 0;
	float eye_y = 8;
	float eye_z = 32;

	// look-at center point
	float look_x = 0;
	float look_y = 0;
	float look_z = 0;

	// camera motion direction
	int step = 0;

	// look angle from the z-axis (in radians)
	float theta = 0;
};