#include "Monster.h"

// SETTERS

/*
* Initializes a new MONSTER object
*
*/
Monster::Monster(std::string starting_name, int health, int speed, std::vector<Move*> moves)
{
	name = starting_name;
	starting_health = health;
	starting_speed = speed;
	current_health = starting_health;
	current_speed = starting_speed;
	move_set = moves;
}

/*
* Changes a MONSTER's speed variable by a certain factor
* Also changes the battle message to be displayed based on 
* how successful that change was
* 
* @param new_speed, the factor by which the speed is being changed by 
*/
void const Monster::change_speed(int new_speed) 
{ 
	if (current_speed > 0)
	{
		if (new_speed > 0) battle_message = name + "'s speed lowered by " + std::to_string(new_speed);
		else if (new_speed < 0) battle_message = name + "'s speed raised by " + std::to_string(new_speed);
		current_speed += new_speed;
	}
	else if (current_speed == 0)
	{
		battle_message = name + " cannot lower their speed anymore!";
	}
	else if (current_speed < 0)
	{
		current_speed == 0;
		battle_message = name + " cannot lower their speed anymore!";
	}
};

/*
* Changes a MONSTER's health variable by a certain factor
* Also changed the battle message to be displayed based on
* how successful that change was
* 
* @param new_health, the factor by which the health is being changed by
*/
void const Monster::change_health(int new_health) 
{ 
	if (current_health == starting_health)
	{
		battle_message = name + " is already at max health ";
		return;
	}
	if (current_health > 0)
	{
		if (new_health > 0) battle_message = name + " was healed for " + std::to_string(new_health);
		else if (new_health < 0) battle_message = name + " was hit for " + std::to_string(new_health);
		current_health += new_health;	
		if (current_health > starting_health) current_health = starting_health;
	}
	else if (current_health == 0)
	{
		battle_message = name + " has fainted!";
		is_fainted = true;
	}
	else if (current_health < 0)
	{
		current_speed == 0;
		battle_message = name + " has fainted!";
		is_fainted = true;
	}
};

/*
* Takes in an opponent MONSTER object and a MOVE object
* Calls the appropiate setters based on what type of MOVE object was
* passed in (ex: a move that damages the opponent is labeled as
* a OPP_HEALTH_CHANGE move because it changes the opponent's health)
* 
* @param opponent, a MONSTER object that serves as your opponent
* @param selected_move, a MOVE objec that serves as the move being executed
*/
void Monster::do_move(Monster& opponent, Move& selected_move)
{
	switch (selected_move.get_move_type())
	{
	case (HEALTH_CHANGE):
		change_health(selected_move.get_health_change());
		break;
	case (OPP_HEALTH_CHANGE):
		opponent.change_health(-selected_move.get_health_change());
		break;
	case (SPEED_CHANGE):
		change_speed(selected_move.get_health_change());
		break;
	case (OPP_SPEED_CHANGE):
		opponent.change_speed(-selected_move.get_health_change());
		break;
	}
}