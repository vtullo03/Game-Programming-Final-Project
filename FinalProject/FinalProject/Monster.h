#pragma once
#define GL_SILENCE_DEPRECATION

#define GL_GLEXT_PROTOTYPES 1
#include <vector>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#include "Move.h"

class Monster
{
private:
	std::string input_file;
	std::string name;
	int starting_speed;
	int starting_health;
	int current_speed = starting_speed;
	int current_health = starting_health;

	std::vector<Move> move_set;

	std::string battle_message;
	bool is_fainted = false;

public:

	void do_move(Monster& opponent, Move& selected_move);
	void build_monster();

	// setters and getters 
	void const change_speed(int new_speed);
	void const change_health(int new_health);
};