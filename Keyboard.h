#pragma once

#include <GL/freeglut.h>
#include <iostream>

class Keyboard {
public:

	// constructor
	Keyboard();

	// desctructor
	~Keyboard();

	// keyboard event listeners
	void keyboardDown(char);
	void keyboardUp(char);
	void specialKeys(int);

	// 127 ASCII keys + 21 special keys 
	/// ASCII reference @ https://theasciicode.com.ar/
	/// special reference @ https://www.opengl.org/resources/libraries/glut/spec3/node54.html
	static const int TOTAL_KEYS = 127 + 21;

	// array of keys, indexed by ASCII
	bool keysDown[TOTAL_KEYS];

private:

};