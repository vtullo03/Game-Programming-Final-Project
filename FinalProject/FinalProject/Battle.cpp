#include "Battle.h"

void Battle::set_battle_UI()
{
	size_t move_list_size = player_monster->get_moves().size();
	m_state.ui = new UI[move_list_size];
	for (size_t i = 0; i < move_list_size; ++i)
	{
		m_state.ui[i].set_ui_type(BUTTON);
		m_state.ui[i].set_position(glm::vec3(0.0f + i, 0.0f, 0.0f));
		m_state.ui[i].set_width(4.0f);
		m_state.ui[i].set_height(4.0f);
		// found c_str() and strcpy_s() in the c++ documentation
		// documentation is truly a programmer's best friend
		std::string filepath = player_monster->get_moves()[i]->get_name() + ".png";
		char* new_filepath = new char[filepath.length() + 1];
		strcpy_s(new_filepath, filepath.length() + 1, filepath.c_str());
		m_state.ui[i].m_texture_id = Utility::load_texture(new_filepath);
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

	size_t move_list_size = player_monster->get_moves().size();
	for (size_t i = 0; i < move_list_size; ++i)
	{
		m_state.ui[i].update(delta_time);
	}
}

void Battle::render(ShaderProgram* program)
{
	size_t move_list_size = player_monster->get_moves().size();
	for (size_t i = 0; i < move_list_size; ++i)
	{
		m_state.ui[i].render(program);
	}
}