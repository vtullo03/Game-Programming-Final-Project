#pragma once

#pragma once
#define GL_SILENCE_DEPRECATION

#define GL_GLEXT_PROTOTYPES 1
#include <vector>
#include <fstream>
#include <iostream>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

enum MoveType { OPP_HEALTH_CHANGE, OPP_SPEED_CHANGE, HEALTH_CHANGE, SPEED_CHANGE };

class Move
{
private:
	std::string name;
	int health_change;
	int speed_change;

	MoveType move_type;

public:
	// setters
	void const set_name(std::string new_name) { name = new_name; };
	void const set_damage(int new_health_change) { health_change = new_health_change; };
	void const st_enemy_speed_change(int new_speed_change) { speed_change = new_speed_change; };
	void const set_move_type(MoveType new_move_type) { move_type = new_move_type; };

	// getters
	const std::string get_name() const { return name; };
	const int get_health_change() const { return health_change; };
	const int get_speed_change() const { return speed_change; };
	const MoveType get_move_type() const { return move_type; };
};