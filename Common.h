#pragma once

#include <string.h>

struct box
{
	// box's positions
	float min_x = 0;
	float max_x = 0;

	float min_y = 0;
	float max_y = 0;

	float min_z = 0;
	float max_z = 0;
};

struct star
{
	// star's position
	float pos_x = 0;
	float pos_y = 0;
	float pos_z = 0;

	// 'collected' or not
	bool show = true;
};