#include "World.h"
#include "Common.h"

/**/
/*
World::World() World::World()

NAME

	World::World - constructor for the World object

SYNOPSIS

	World::World();

DESCRIPTION

	This function will create a new World object.

RETURNS

	No return value.

AUTHOR

	Kevin Bagnato
*/
/**/
World::World()
{
	//loadTexture();
	reset();
}


/**/
/*
World::~World() World::~World()

NAME

	World::~World - destructor for the World object

SYNOPSIS

	World::~World();

DESCRIPTION

	This function currently does nothing.

RETURNS

	No return value.

AUTHOR

	Kevin Bagnato
*/
/**/
World::~World()
{
	// do nothing
}


/**/
/*
World::drawWorld() World::drawWorld()

NAME

	World::drawWorld - draw the world on screen

SYNOPSIS

	World::drawWorld(int level);
		int level		->	the level to draw

DESCRIPTION

	This function will call the appropriate fucntion to draw
	a specific world, typically including the floor, boxes, and stars.

RETURNS

	No return value.

AUTHOR

	Kevin Bagnato
*/
/**/
void World::drawWorld(int level)
{
	switch (level)
	{
	case 1:
		worldOne();
		break;
	case 2:
		worldTwo();
		break;
	case 3:
		worldThree();
		break;
	default:
		worldZero();
	}
}

void World::worldZero()
{
	drawFloor();
	totalStars = 0;
	totalBoxes = 0;
}

void World::worldOne()
{
	drawCheckeredFloor();

	// texture id = ...
	// gravity = ...
	
	//// BOXES ////
	totalBoxes = 2;

	// orange box size 10x2x5 (width height depth) with center (5, 0, 5)
	// jump onto
	glColor3f(0.8, 0, 0.2);
	drawBox(0, 10, 2, 5, 5, 0, 5);

	// blue box - roll onto
	glColor3f(0, 0.2, 255);
	drawBox(1, 5, 5, 5, 0, 0, -5);

	//// STARS ////
	totalStars = 3;
	
	// close to home
	drawStar(0, -5, 5, 5);
	
	// player has to jump
	drawStar(1, 10, 10, 10);

	// in the corner
	drawStar(2, -35, 5, -35);
}

void World::worldTwo()
{
	worldSize = 100;
	drawFloor();

	totalStars = 0;
	
	//// BOXES ////
	totalBoxes = 5;

	// red box
	glColor3f(1, 0, 0);
	drawBox(0, 5, 5, 5, 0, 0, -5);

	// orange box
	glColor3f(1, 0.6, 1);
	drawBox(1, 5, 5, 5, 0, 0, -15);

	// yellow box
	glColor3f(1, 1, 0);
	drawBox(2, 5, 5, 5, 0, 0, -25);

	// green box
	glColor3f(0, 1, 0);
	drawBox(3, 5, 5, 5, 0, 0, -35);

	// blue box
	glColor3f(0, 0, 1);
	drawBox(4, 5, 5, 5, 0, 0, -45);
}

void World::worldThree()
{
	worldSize = 25;
	drawFloor();

	totalStars = 1;
	totalBoxes = 1;

	// rail
	drawBox(0, 1, 1, 15, 0, 2, -5);

	// star at end
	drawStar(0, 0, 5, -15);

}


/**/
/*
World::drawFloor() World::drawFloor()

NAME

	World::drawFloor - draw the floor

SYNOPSIS

	World::drawFloor();

DESCRIPTION

	This function will draw a floor of orange gridlines.

RETURNS

	No return value.

AUTHOR

	Kevin Bagnato
*/
/**/
void World::drawFloor()
{
	glDisable(GL_LIGHTING);

	// floor is orange grid
	glColor3f(1., 0.5, 0.);

	for (int i = -worldSize; i <= worldSize; i++) {
		glBegin(GL_LINES);			//A set of grid lines on the xz-plane
		glVertex3f(-worldSize, 0, i);
		glVertex3f(worldSize, 0, i);
		glVertex3f(i, 0, -worldSize);
		glVertex3f(i, 0, worldSize);
		glEnd();
	}

	glEnable(GL_LIGHTING);
}


/**/
/*
World::drawCheckeredFloor() World::drawCheckeredFloor()

NAME

	World::drawCheckeredFloor - draw a checkered floor

SYNOPSIS

	World::drawCheckeredFloor();

DESCRIPTION

	This function will draw a checkered floor of alternating
	lime green and green sqaures.

RETURNS

	No return value.

AUTHOR

	Kevin Bagnato
*/
/**/
void World::drawCheckeredFloor()
{
	glDisable(GL_LIGHTING);
	bool color = false;

	glBegin(GL_QUADS);
	for (int i = -worldSize; i <= worldSize; i++)
	{
		for (int j = -worldSize; j <= worldSize; j++)
		{
			if (color)
			{
				glColor3f(0.2, 0.8, 0);
			}
			else
			{
				glColor3f(0.8, 1.0, 0.6);
			}
			glVertex3f(i, 0, j);
			glVertex3f(i, 0, j + 1);
			glVertex3f(i + 1, 0, j + 1);
			glVertex3f(i + 1, 0, j);
			color = !color;
		}
	}
	glEnd();

	glEnable(GL_LIGHTING);
}


/**/
/*
World::drawBox() World::drawBox()

NAME

	World::drawBox - draw a box in the world

SYNOPSIS

	World::drawBox(int id, float width, float height, float depth, float x, float y, float z);
		int id			->	the box ID number
		float width		->	the width of the box
		float height	->	the height of the box
		float depth		->	the depth of the box
		float x			->	the center of the box along the X axis
		float y			->	the bottom of the box along the Y axis
		float z			->	the center of the box along the Z axis

DESCRIPTION

	This function will draw a box of a given size at a given location.
	The Y value is set to the bottom of the box (opposed to the center like X and Z)
	to ease programming for box height. Boxes have bounds for collision detection.

RETURNS

	No return value.

AUTHOR

	Kevin Bagnato
*/
/**/
void World::drawBox(int id, float width, float height, float depth, float x, float y, float z)
{
	// draw box in world
	glPushMatrix();
	{
		// move to (x, y, z)
		glTranslatef(x, y, z);

		// raise to floor
		glTranslatef(0, height / 2, 0);

		// resize box
		glScalef(width, height, depth);
		glutSolidCube(1);
	}
	glPopMatrix();


	// set x bounds
	boxes[id].min_x = x - width / 2;
	boxes[id].max_x = x + width / 2;

	// set z bounds
	boxes[id].min_z = z - depth / 2;
	boxes[id].max_z = z + depth / 2;

	// set height bound
	boxes[id].min_y = y - height;
	boxes[id].max_y = y + height;
}


/**/
/*
World::drawPillar() World::drawPillar()

NAME

	World::drawPillar - draw a pillar in the world

SYNOPSIS

	World::drawBox(float x, float z);
		float x			->	the center of the pillar along the X axis
		float z			->	the center of the pillar along the Z axis

DESCRIPTION

	This function will draw a pillar in the given position, extending seemingly
	infinitely high. No collision detection. Useful for debugging.

RETURNS

	No return value.

AUTHOR

	Kevin Bagnato
*/
/**/
void World::drawPillar(float x, float z)
{
	glColor4f(.8, .8, 0, 0);

	glPushMatrix();
	{
		// move to given position
		glTranslatef(x, 0, z);

		// raise to floor
		glTranslatef(0, 100, 0);

		// orient upwards
		glRotatef(90, 1, 0, 0);
		glutSolidCylinder(0.2, 100, 10, 10);
	}
	glPopMatrix();
}


/**/
/*
World::drawWall() World::drawWall()

NAME

	World::drawWall - draw a wall in the world

SYNOPSIS

	World::drawBox(float x1, float z1, float x2, float z2);
		float x1		->	the X coordinate of the first side
		float z1		->	the Z coordinate of the first side
		float x2		->	the X coordinate of the second side
		float z2		->	the Z coordinate of the second side

DESCRIPTION

	This function will draw a wall from (x1, z1) to (x2, z2). No collision detection,
	unlike a box.

RETURNS

	No return value.

AUTHOR

	Kevin Bagnato
*/
/**/
void World::drawWall(float x1, float z1, float x2, float z2)
{
	glPushMatrix();
	{

		float len = sqrt((x2 - x1) * (x2 - x1) + (z2 - z1) * (z2 - z1));
		glTranslatef(0, 25, 0);
		glScalef(len, 50, 1);
		glutSolidCube(1);
	}
	glPopMatrix();
}


/**/
/*
World::drawStar() World::drawStar()

NAME

	World::drawStar - draw a star in the world

SYNOPSIS

	World::drawStar(int id, float x, float  y, float z)
		int id			->	the star ID number
		float x			->	the center of the star along the X axis
		float y			->	the bottom of the star along the Y axis
		float z			->	the center of the star along the Z axis

DESCRIPTION

	This function will draw a spinnig star at (x, y, z).

RETURNS

	No return value.

AUTHOR

	Kevin Bagnato
*/
/**/
void World::drawStar(int id, float x, float  y, float z)
{
	glEnable(GL_LIGHTING);
	glColor3f(255, 255, 0);

	if (stars[id].show)
	{
		glPushMatrix();
		{
			// move the star to the given position
			glTranslatef(x, y, z);

			// spin the coin
			glRotatef(starRotation, 0, 1, 0);

			// draw the star
			glutSolidTorus(0.5, 1, 10, 10);

		}
		glPopMatrix();

		// save star position for collision detection
		stars[id].pos_x = x;
		stars[id].pos_y = y;
		stars[id].pos_z = z;

		// spin the star
		starRotation++;
		if (starRotation == 360)
		{
			starRotation = 0;
		}
	}
}


/**/
/*
World::getScore() World::getScore()

NAME

	World::getScore - get the Player's score

SYNOPSIS

	World::getScore();

DESCRIPTION

	This function will return the Player's score
	(number of stars collected) as a string.

RETURNS

	The Player's score.

AUTHOR

	Kevin Bagnato
*/
/**/
std::string World::score_toString()
{
	std::string s = "Score: " + std::to_string(getScore());
	return s;
}


/**/
/*
World::getScore() World::getScore()

NAME

	World::getScore - get the Player's score

SYNOPSIS

	World::getScore();

DESCRIPTION

	This function will return the Player's score
	(number of stars collected) as an integer.

RETURNS

	The Player's score.

AUTHOR

	Kevin Bagnato
*/
/**/
int World::getScore()
{
	int score = 0;

	for (int i = 0; i < totalStars; i++)
	{
		if (!stars[i].show)
		{
			score++;
		}
	}

	return score;
}


/**/
/*
World::outlineBoxes() World::outlineBoxes()

NAME

	World::outlineBoxes - outline all boxes with pillar

SYNOPSIS

	World::outlineBoxes();

DESCRIPTION

	This function will draw a vertical pillar at each corner of each box.
	Useful for debugging.

RETURNS

	No return value.

AUTHOR

	Kevin Bagnato
*/
/**/
void World::outlineBoxes()
{
	for (int i = 0; i < totalBoxes; i++)
	{
		drawPillar(boxes[i].min_x, boxes[i].min_z);
		drawPillar(boxes[i].min_x, boxes[i].max_z);

		drawPillar(boxes[i].max_x, boxes[i].min_z);
		drawPillar(boxes[i].max_x, boxes[i].max_z);

	}
}


/**/
/*
World::reset() World::reset()

NAME

	World::reset - reset the World's stars

SYNOPSIS

	World::reset();

DESCRIPTION

	This function will set all stars in the world to visible.

RETURNS

	No return value.

AUTHOR

	Kevin Bagnato
*/
/**/
void World::reset()
{
	for (int i = 0; i < totalStars; i++)
	{
		stars[i].show = true;
	}
}