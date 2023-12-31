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
	std::string name;
	int starting_speed;
	int starting_health;
	int current_speed = starting_speed;
	int current_health = starting_health;

	std::vector<Move*> move_set;

	std::string battle_message;
	bool is_fainted = false;

public:

	Monster(std::string starting_name, int health, int speed, std::vector<Move*> moves);
	void do_move(Monster* opponent, Move* selected_move);
	Move* rand_select_move();

	// setters and getters 
	void const change_speed(int new_speed);
	void const change_health(int new_health);
	int const get_speed() { return current_speed; };
	int const get_health() { return current_health; };
	const std::vector<Move*> const get_moves() { return move_set; };
	const std::string const get_name() { return name; }; // for debugging currently -- maybe delete later?
};