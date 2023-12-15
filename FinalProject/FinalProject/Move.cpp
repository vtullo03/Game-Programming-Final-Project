#include "Move.h"

/*
* MOVE constructor class
* The MOVE class is used by MONSTER classes to determines things like
* If it's a damage, healing, or status effect move
* Also determines the strength of said type of move
*/
Move::Move(std::string starting_name, int health, int speed, MoveType type)
{
	name = starting_name;
	health_change = health;
	speed_change = speed;
	move_type = type;
}