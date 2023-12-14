#include "Move.h"

Move::Move(std::string starting_name, int health, int speed, MoveType type)
{
	name = starting_name;
	health_change = health;
	speed_change = speed;
	move_type = type;
}