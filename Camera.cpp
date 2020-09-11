#include "Camera.h"

/**/
/*
Camera::move() Camera::move()

NAME

	Camera::move - move the camera

SYNOPSIS

	Camera::move(bool keys[]);
		bool keys[]		->	an array of values representing each key on the
		keyboard (special keys excluded) where a value of true represents the
		key is currently pressed down, and a value of false represents the
		key is currently released.

DESCRIPTION

	This function first checks what keys are pressed and updates the camera's
	position and orientation appropriately. 

	'i' will move the camera forward
	'k' will move the camera backward
	'j' will turn the camera left
	'l' will turn the camera right
	'u' will raise the camera
	'o' will lower the camera


RETURNS

	No return value.

AUTHOR

	Kevin Bagnato
*/
/**/
void Camera::move(bool keys[])
{
	// move camera with IJKLUO
	if (keys['i']) {
		step = 1;
	}
	if (keys['k']) {
		step = -1;
	}
	if (keys['j']) {
		theta += 2;
	}
	if (keys['l']) {
		theta -= 2;
	}
	if (keys['u']) {
		eye_y += 0.1;
	}
	if (keys['o']) {
		eye_y -= 0.1;
	}

	// convert theta from degrees to radians
	float thetaRad = theta * 3.145 / 180;

	// camera movement direction
	float dir_x = -sin(thetaRad);
	float dir_y = 0;
	float dir_z = -cos(thetaRad);

	// camera position
	float s = 0.5, d = 2;
	eye_x = eye_x + step * s * dir_x;
	eye_z = eye_z + step * s * dir_z;

	// camera orientation
	look_x = eye_x + d * dir_x;
	look_y = eye_y + d * dir_y;
	look_z = eye_z + d * dir_z;

	// init matrix for buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// set camera position and look-at point
	gluLookAt(eye_x, eye_y, eye_z, look_x, look_y, look_z, 0, 1, 0);

	// reset camera step to prevent the camera from always moving
	step = 0;
}


/**/
/*
Camera::reset() Camera::reset()

NAME

	Camera::reset - reset the camera's position and orientation

SYNOPSIS

	Camera::reset();

DESCRIPTION

	This function will set the camera's position to its original location,
	point it towards the center of the world, and clear its look-at angle.

RETURNS

	No return value.

AUTHOR

	Kevin Bagnato
*/
/**/
void Camera::reset()
{
	eye_x = 0;
	eye_y = 8;
	eye_z = 32;
	look_x = 0;
	look_y = 0;
	look_z = 0;
	theta = 0;
}