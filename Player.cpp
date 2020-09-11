#include "Player.h"

/**/
/*
Player::drawPlayer() Player::drawPlayer()

NAME

	Player::drawPlayer - draw the player (unicycle) on the screen
	
SYNOPSIS

	Player::drawPlayer();

DESCRIPTION

	This function draws the player on the screen at its position
	with correct orientation.

RETURNS

	No return value.

AUTHOR

	Kevin Bagnato
*/
/**/
void Player::drawPlayer()
{
	glPushMatrix();
	{
		// set player position
		glTranslatef(pos_x, pos_y, pos_z);

		// set player orientation
		glRotatef(theta + 90, 0, 1, 0);

		// raise the whole unicycle to floor level
		glTranslatef(0, 2, 0);
		{
			drawUnicycle();
		}
	}
	glPopMatrix();
}


/**/
/*
Player::move() Player::move()

NAME

	Player::move - move the player

SYNOPSIS

	Player::move(bool keys[]);
		bool keys[]		->	an array of values representing each key on the
		keyboard (special keys excluded) where a value of true represents the
		key is currently pressed down, and a value of false represents the
		key is currently released.

DESCRIPTION

	This function saves the inital state of the player, checks what keys are
	pressed, and updates the player's position, orientation, and collision points
	appropriately. 

	'w' will move the camera forward
	's' will move the camera backward
	'a' will turn the camera left
	'd' will turn the camera right
	'[spacebar]' will make the player jump

RETURNS

	No return value.

AUTHOR

	Kevin Bagnato
*/
/**/
void Player::move(bool keys[])
{
	lastPos_x = pos_x;
	lastPos_y = pos_y;
	lastPos_z = pos_z;
	lastTheta = theta;
	lastThetaRad = thetaRad;

	// adjust forward direction with A/D keys
	if (keys['a']) {
		theta += 2.5;
	}
	if (keys['d']) {
		theta -= 2.5;
	}

	// convert theta from degrees to radians
	thetaRad = theta * (float) 3.145 / 180;

	// calculate normalized forward vector
	float dir_x = -sin(thetaRad);
	float dir_z = -cos(thetaRad);
	float length = sqrt(dir_x * dir_x + dir_z * dir_z);
	dir_x = dir_x / length;
	dir_z = dir_z / length;

	// adjust velocity with W/S keys
	if (keys['w']) {
		vel_x += (float) 0.1 * dir_x;
		vel_z += (float) 0.1 * dir_z;

	}
	if (keys['s']) {
		vel_x -= (float) 0.1 * dir_x;
		vel_z -= (float) 0.1 * dir_z;
	}

	// add friction / drag
	vel_x *= (float) 0.8;
	vel_z *= (float) 0.8;

	// jumping
	if (keys[' '] && grounded)
	{
		vel_y += 0.5;
		grounded = false;
	}

	// fall if not on ground
	if (!grounded)
	{
		vel_y -= (float) 0.03;
	}

	// update position
	pos_x += vel_x;
	pos_y += vel_y;
	pos_z += vel_z;

	// spin spokes when moving
	float vel = sqrt(vel_x * vel_x + vel_z * vel_z);
	spokesRotation += vel * 20;
	// reset int to prevent overflow
	if (spokesRotation == 360)
	{
		spokesRotation = 0;
	}

	// update bounding box points
	frontWheel_x = pos_x + 2 * -sin(thetaRad);
	frontWheel_y = pos_y;
	frontWheel_z = pos_z + 2 * -cos(thetaRad);

	backWheel_x = pos_x - 2 * -sin(thetaRad);
	backWheel_y = pos_y;
	backWheel_z = pos_z - 2 * -cos(thetaRad);
}


/**/
/*
Player::reset() Player::reset()

NAME

	Player::reset - reset the Player's position, orientation, and velocity

SYNOPSIS

	Player::reset();

DESCRIPTION

	This function will set the Player's position to its original location,
	clear its look-at angle, and clear its velocity.

RETURNS

	No return value.

AUTHOR

	Kevin Bagnato
*/
/**/
void Player::reset()
{
	pos_x = 0;
	pos_y = 0;
	pos_z = 0;
	theta = 0;
	thetaRad = 0;
	vel_x = 0;
	vel_y = 0;
	vel_z = 0;
}


/**/
/*
Player::keepInside() Player::keepInside()

NAME

	Player::keepInside - keep the player's position to within the given world size

SYNOPSIS

	Player::keepInside(float w);
		float w		->	the length of the world (world's are square)

DESCRIPTION

	This function will check if the Player's position is greater than
	the world size, and restrict the movement if so.

RETURNS

	No return value.

AUTHOR

	Kevin Bagnato
*/
/**/
void Player::keepInside(int  w)
{
	if (pos_x > w)
	{
		pos_x = (float) w;
	}
	if (pos_x < -w)
	{
		pos_x = (float)-w;
	}
	if (pos_z > w)
	{
		pos_z = (float)w;
	}
	if (pos_z < -w)
	{
		pos_z = (float) -w;
	}

	// prevent player from falling through floor
	if (pos_y < 0) {
		pos_y = 0;
		vel_y = 0;
		grounded = true;
	}
}


/**/
/*
Player::revertPos() Player::revertPos()

NAME

	Player::revertPos - revert the Player's position to its last

SYNOPSIS

	Player::revertPos();

DESCRIPTION

	This function will set the Player's position and look-at direction
	to its previos state (stored in Player::move). It is used to prevent
	the player from intersecting with boxes.

RETURNS

	No return value.

AUTHOR

	Kevin Bagnato
*/
/**/
void Player::revertPos()
{
	pos_x = lastPos_x;
	pos_y = lastPos_y;
	pos_z = lastPos_z;
	thetaRad = lastThetaRad;
}


/**/
/*
Player::drawUnicycle() Player::drawUnicycle()

NAME

	Player::drawUnicycle - draw the unicycle on the screen

SYNOPSIS

	Player::drawUnicycle();

DESCRIPTION

	This function will draw the unicycle on the screen by rendering each 
	part (tire, spokes with rotation, seat post, and seat).

RETURNS

	No return value.

AUTHOR

	Kevin Bagnato
*/
/**/
void Player::drawUnicycle()
{
	//// TIRE ////
	// black
	glColor3f(0.1, 0.1, 0.1);
	glPushMatrix();
	{
		glutSolidTorus(0.4, 2.0, 8, 20);
	}
	glPopMatrix();


	//// SPOKES ////
	// silver color
	glColor3f(0.9, 0.9, 0.9);
	glPushMatrix();
	{
		// orient the spokes correctly
		glRotatef(90, 0, 1, 0);

		// only spin spokes when moving
		//if (spokesMoving) {
		glRotatef(spokesRotation, 1, 0, 0);
		//}

		// draw multiple spokes
		for (int i = 0; i < 10; i++) {
			glPushMatrix();
			{
				glRotatef(i * 36, 1, 0, 0);
				glutSolidCylinder(0.1, 2, 4, 1);
			}
			glPopMatrix();
		}
	}
	glPopMatrix();

	//// SEAT POST ////
	// red
	glColor3f(1, 0, 0);
	glPushMatrix();
	{
		// raise seat to meet spokes center
		glTranslatef(0, 5.6, 0);
		// orient upwards
		glRotatef(90, 1, 0, 0);
		glutSolidCylinder(0.25, 6, 4, 1);
	}
	glPopMatrix();

	//// SEAT ////
	// blue-green
	glColor3f(0, 0.8, 0.5);
	glPushMatrix();
	{
		// raise seat to top of post
		glTranslatef(0, 5.5, 0);
		glScalef(2.5, 0.5, 1);
		glutSolidCube(1);
	}
	glPopMatrix();

	//// SEAT DIRECTION ////
	glPushMatrix();
	{
		glTranslatef(1.5, 5.5, 0);
		glScalef(1, 0.5, 1);
		glutSolidTetrahedron();
	}
	glPopMatrix();
}


/**/
/*
Player::drawBoundingBox() Player::drawBoundingBox()

NAME

	Player::drawBoundingBox - draw a bounding box around the player

SYNOPSIS

	Player::drawBoundingBox();

DESCRIPTION

	This function will draw a wireframe box around the player.
	Useful for debugging.

RETURNS

	No return value.

AUTHOR

	Kevin Bagnato
*/
/**/
void Player::drawBoundingBox()
{
	// make box red
	glColor3f(1, 0, 0);

	// translate and rotate box to player's position
	glPushMatrix();
	{
		glTranslatef(pos_x, pos_y, pos_z);
		glRotatef(theta, 0, 1, 0);

		// draw wireframe box
		glPushMatrix();
		{
			// raise box to floor level
			glTranslatef(0, size_y / 2, 0);
			
			glScalef(size_x, size_y, size_z);
			glutWireCube(1);
		}
		glPopMatrix();
	}
	glPopMatrix();
}


/**/
/*
Player::getPos_x() Player::getPos_x()

NAME

	Player::getPos_x - get the Player's X coordinate

SYNOPSIS

	Player::getPos_x();

DESCRIPTION

	This function will return the Player's X coordinate.

RETURNS

	The Player's X coordinate.

AUTHOR

	Kevin Bagnato
*/
/**/
float Player::getPos_x()
{
	return pos_x;
}


/**/
/*
Player::getPos_y() Player::getPos_y()

NAME

	Player::getPos_y - get the Player's Y coordinate

SYNOPSIS

	Player::getPos_y();

DESCRIPTION

	This function will return the Player's Y coordinate.

RETURNS

	The Player's Y coordinate.

AUTHOR

	Kevin Bagnato
*/
/**/
float Player::getPos_y()
{
	return pos_y;
}


/**/
/*
Player::getPos_z() Player::getPos_z()

NAME

	Player::getPos_z - get the Player's Z coordinate

SYNOPSIS

	Player::getPos_z();

DESCRIPTION

	This function will return the Player's Z coordinate.

RETURNS

	The Player's Z coordinate.

AUTHOR

	Kevin Bagnato
*/
/**/
float Player::getPos_z()
{
	return pos_z;
}


/**/
/*
Player::setPos_y() Player::setPos_y()

NAME

	Player::setPos_y - set the Player's Y coordinate

SYNOPSIS

	Player::getPos_y(float y);
		float y		->	the value to set for the Player's Y coordinate

DESCRIPTION

	This function will set the Player's Y coordinate.

RETURNS

	No return value.

AUTHOR

	Kevin Bagnato
*/
/**/
void Player::setPos_y(float y)
{
	pos_y = y;
}


/**/
/*
Player::setVel_y() Player::setVel_y()

NAME

	Player::setVel_y - set the Player's Y velocity

SYNOPSIS

	Player::setVel_y(float y);
		float y		->	the value to set for the Player's Y velocity

DESCRIPTION

	This function will set the Player's Y velocity.

RETURNS

	No return value.

AUTHOR

	Kevin Bagnato
*/
/**/
void Player::setVel_y(float y)
{
	vel_y = y;
}


/**/
/*
Player::getForward() Player::getForward()

NAME

	Player::getForward - get the Player's look-at direction

SYNOPSIS

	Player::getForward();

DESCRIPTION

	This function will return the Player's look-at direction in radians.

RETURNS

	The Player's look-at direction in radians.

AUTHOR

	Kevin Bagnato
*/
/**/
float Player::getForward()
{
	return thetaRad;
}


/**/
/*
Player::getGrounded() Player::getGrounded()

NAME

	Player::getGrounded - get the Player's grounded status

SYNOPSIS

	Player::getGrounded();

DESCRIPTION

	This function will return the Player's grounded status.

RETURNS

	The Player's grounded status.

AUTHOR

	Kevin Bagnato
*/
/**/
bool Player::getGrounded()
{
	return grounded;
}

/**/
/*
Player::setGrounded() Player::setGrounded()

NAME

	Player::setGrounded - set the Player's grounded status

SYNOPSIS

	Player::setGrounded(bool g);
		bool g		->	the value to set for the Player's grounded status

DESCRIPTION

	This function will set the Player's grounded status to the given value.

RETURNS

	No return value.

AUTHOR

	Kevin Bagnato
*/
/**/
void Player::setGrounded(bool g)
{
	grounded = g;
	
	if (g)
	{
		vel_y = 0;
	}
}


/**/
/*
Player::pos_toString() Player::pos_toString()

NAME

	Player::pos_toString - get the Player's position as a string

SYNOPSIS

	Player::pos_toString();

DESCRIPTION

	This function will return the Player's three coordinate position.

RETURNS

	The Player's position as a string.

AUTHOR

	Kevin Bagnato
*/
/**/
string Player::pos_toString()
{
	string s = "pos_x: " + to_string(pos_x) + " pox_y: " + to_string(pos_y) + " pos_z: " + to_string(pos_z);
	return s;
}


/**/
/*
Player::vel_toString() Player::vel_toString()

NAME

	Player::vel_toString - get the Player's velocity as a string

SYNOPSIS

	Player::vel_toString();

DESCRIPTION

	This function will return the Player's three coordinate velocity.

RETURNS

	The Player's velocity as a string.

AUTHOR

	Kevin Bagnato
*/
/**/
string Player::vel_toString()
{
	string s = "vel_x: " + to_string(vel_x) + " vel_y: " + to_string(vel_y) + " vel_z: " + to_string(vel_z);
	return s;
}


/**/
/*
Player::frontWheel_toString() Player::frontWheel_toString()

NAME

	Player::frontWheel_toString - get the player's front wheel position

SYNOPSIS

	Player::frontWheel_toString();

DESCRIPTION

	This function will return the Player's front wheel two coordinate position.
	Useful for debugging.

RETURNS

	The Player's front wheel position as a string.

AUTHOR

	Kevin Bagnato
*/
/**/
string Player::frontWheel_toString()
{
	string s = "frontWheel_x: " + to_string(frontWheel_x) + " frontWheel_z: " + to_string(frontWheel_z);
	return s;
}


/**/
/*
Player::backWheel_toString() Player::backWheel_toString()

NAME

	Player::backWheel_toString - get the player's back wheel position

SYNOPSIS

	Player::backWheel_toString();

DESCRIPTION

	This function will return the Player's back wheel two coordinate position.
	Useful for debugging.

RETURNS

	The Player's back wheel position as a string.

AUTHOR

	Kevin Bagnato
*/
/**/
string Player::backWheel_toString()
{
	string s = "backWheel_x: " + to_string(backWheel_x) + " backWheel_z: " + to_string(backWheel_z);
	return s;
}