#pragma once

#include "Keyboard.h"

/**/
/*
Keyboard::Keyboard() Keyboard::Keyboard()

NAME

	Keyboard::Keyboard - Keyboard constructor function

SYNOPSIS

	Keyboard::Keyboard();

DESCRIPTION

	This function creates a Keyboard object with all keys set to released
	by default.

RETURNS

	No return value.

AUTHOR

	Kevin Bagnato
*/
/**/
Keyboard::Keyboard()
{
	for (int i = 0; i < TOTAL_KEYS; i++)
	{
		keysDown[i] = false;
	}
}


/**/
/*
Keyboard::~Keyboard() Keyboard::~Keyboard()

NAME

	Keyboard::~Keyboard() - destructor for the Keyboard object

SYNOPSIS

	Keyboard::~Keyboard();

DESCRIPTION

	This function currently does nothing.

RETURNS

	No return value.

AUTHOR

	Kevin Bagnato
*/
/**/
Keyboard::~Keyboard()
{
	// do nothing
}


/**/
/*
Keyboard::keyboardDown() Keyboard::keyboardDown()

NAME

	Keyboard::keyboardDown - event handler for keyboard key presses

SYNOPSIS

	Keyboard::keyboardDown(char key);
		char key		->	the key pressed

DESCRIPTION

	This function sets the appropriate index in 'keysDown[]' to be
	true, representing the key is pressed down. It will exit the
	game is the Escape key is pressed.

RETURNS

	No return value.

AUTHOR

	Kevin Bagnato
*/
/**/
void Keyboard::keyboardDown(char key)
{
	keysDown[key] = true;

	// quit game with ESC
	if (key == 27)
	{
		exit(0);
	}
}


/**/
/*
Keyboard::keyboardUp() Keyboard::keyboardUp()

NAME

	Keyboard::keyboardUp - event handler for keyboard key released

SYNOPSIS

	Keyboard::keyboardUp(char key);
		char key		->	the key released

DESCRIPTION

	This function sets the appropriate index in 'keysDown[]' to be
	false, representing the key is released.

RETURNS

	No return value.

AUTHOR

	Kevin Bagnato
*/
/**/
void Keyboard::keyboardUp(char key)
{
	keysDown[key] = false;
}


/**/
/*
Keyboard::specialKeys() Keyboard::specialKeys()

NAME

	Keyboard::specialKeys - event handler for keyboard special key presses

SYNOPSIS

	Keyboard::specialKeys(int key);
		int key		->	the key pressed

DESCRIPTION

	This function handles special keyboard key presses. It currently does nothing.

RETURNS

	No return value.

AUTHOR

	Kevin Bagnato
*/
/**/
void Keyboard::specialKeys(int key)
{
	// do nothing
}