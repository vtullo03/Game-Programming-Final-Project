#include "Battle.h"

void Battle::set_battle_UI()
{
	int move_list_size = player_monster->get_moves().size();
	m_state.ui = new UI[move_list_size];
	for (size_t i = 0; i < move_list_size; ++i)
	{
		m_state.ui[i].set_ui_type(BUTTON);
		m_state.ui[i].set_position(glm::vec3(0.0f + i, 0.0f, 0.0f));
	}
}

void Battle::simulate_battle()
{
	switch (battle_state)
	{
	case PLAYERTURN:
		break;
	case PLAYEREXECUTE:
		break;
	case OPPTURN:
		break;
	case OPPEXECUTE:
		break;
	case END:
		break;
	}
}

void Battle::initialise()
{
	// set battle monsters will be called by an external force
	set_battle_UI(); // therefore we just execute this line
}

void Battle::update(float delta_time)
{
	simulate_battle();
}

void render(ShaderProgram* program)
{

}