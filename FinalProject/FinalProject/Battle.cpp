#include "Battle.h"

const char FONT_FILEPATH[] = "font.png";

Battle::~Battle()
{
	delete m_state.map;
	delete m_state.player;
	delete m_state.opp_monsters;
	delete[] m_state.ui;
}

void Battle::set_battle_UI()
{
	size_t move_list_size = player_monster->get_moves().size();
	std::cout << move_list_size;
	m_state.ui = new UI[move_list_size + 2];
	Scene::set_button_count(move_list_size);

	for (size_t i = 0; i < move_list_size; ++i)
	{
		m_state.ui[i].set_ui_type(BUTTON);
		m_state.ui[i].set_position(glm::vec3(-3.5f + (i * 2.75f), -2.0f, 0.0f));
		m_state.ui[i].set_width(4.0f);
		m_state.ui[i].set_height(4.0f);
		// found c_str() and strcpy_s() in the c++ documentation
		// documentation is truly a programmer's best friend
		// converts a string into a const array of chars
		std::string filepath = player_monster->get_moves()[i]->get_name() + ".png";
		char* new_filepath = new char[filepath.length() + 1];
		strcpy_s(new_filepath, filepath.length() + 1, filepath.c_str());
		m_state.ui[i].m_texture_id = Utility::load_texture(new_filepath);
	}

	// player ui
	m_state.ui[move_list_size].set_ui_type(BUTTON);
	m_state.ui[move_list_size].set_position(glm::vec3(-2.75f, 2.0f, 0.0f));
	m_state.ui[move_list_size].set_width(4.0f);
	m_state.ui[move_list_size].set_height(4.0f);
	std::string player_filepath = player_monster->get_name() + ".png";
	char* new_player_filepath = new char[player_filepath.length() + 1];
	strcpy_s(new_player_filepath, player_filepath.length() + 1, player_filepath.c_str());
	m_state.ui[move_list_size].m_texture_id = Utility::load_texture(new_player_filepath);

	// enemy ui
	m_state.ui[move_list_size + 1].set_ui_type(BUTTON);
	m_state.ui[move_list_size + 1].set_position(glm::vec3(2.75f, 2.0f, 0.0f));
	m_state.ui[move_list_size + 1].set_width(4.0f);
	m_state.ui[move_list_size + 1].set_height(4.0f);
	std::string opp_filepath = opp_monster->get_name() + ".png";
	char* new_opp_filepath = new char[opp_filepath.length() + 1];
	strcpy_s(new_opp_filepath, opp_filepath.length() + 1, opp_filepath.c_str());
	m_state.ui[move_list_size + 1].m_texture_id = Utility::load_texture(new_opp_filepath);
}

void Battle::set_battle_monsters(Monster* player, Monster* opponent)
{
	player_monster = player;
	opp_monster = opponent;
	set_battle_UI();

	if (player_monster->get_speed() > opp_monster->get_speed()) battle_state = PLAYERTURN;
	else if (player_monster->get_speed() < opp_monster->get_speed()) battle_state = OPPTURN;
}

void Battle::simulate_battle()
{
	switch (battle_state)
	{
	case PLAYERTURN:
		player_turn = true;
		if (player_inputted) battle_state = PLAYEREXECUTE;
		break;
	case PLAYEREXECUTE:
		player_turn = false;
		player_inputted = false;
		player_monster->do_move(opp_monster, player_monster->get_moves()[button_index]);
		if (player_monster->get_health() == 0) battle_state = END;
		else battle_state = OPPTURN;
		break;
	case OPPTURN:
		opp_monster->do_move(player_monster, opp_monster->rand_select_move());
		if (opp_monster->get_health() == 0) battle_state = END;
		else battle_state = PLAYERTURN;
		break;
	case END:
		battle_ended = true;
		break;
	}
}

void Battle::initialise()
{
	if (player_monster->get_speed() > opp_monster->get_speed()) battle_state = PLAYERTURN;
	else if (player_monster->get_speed() < opp_monster->get_speed()) battle_state = OPPTURN;
}

void Battle::update(float delta_time)
{
	simulate_battle();

	size_t move_list_size = player_monster->get_moves().size();
	for (size_t i = 0; i < move_list_size + 2; ++i)
	{
		m_state.ui[i].update(delta_time);
	}
	for (size_t i = 0; i < move_list_size; ++i)
	{
		if (i == button_index)
		{
			m_state.ui[i].button_selected();
			m_state.ui[i].button_change_size();
		}
		else
		{
			m_state.ui[i].button_unselected();
		}
	}
}

void Battle::render(ShaderProgram* program)
{
	size_t move_list_size = player_monster->get_moves().size();
	if (player_monster && opp_monster)
	{
		Utility::draw_text(program, Utility::load_texture(FONT_FILEPATH), player_monster->get_name(), 0.25f,
			-0.045f, glm::vec3(-3.0f, -0.75f, 0.0f));
		Utility::draw_text(program, Utility::load_texture(FONT_FILEPATH), opp_monster->get_name(), 0.25f,
			-0.045f, glm::vec3(0.75f, -0.75f, 0.0f));
		std::string player_status = "Health: " + std::to_string(player_monster->get_health());
		Utility::draw_text(program, Utility::load_texture(FONT_FILEPATH), player_status, 0.25f,
			-0.045f, glm::vec3(-3.0f, -1.75f, 0.0f));
		std::string opp_status = "Health: " + std::to_string(opp_monster->get_health());
		Utility::draw_text(program, Utility::load_texture(FONT_FILEPATH), opp_status, 0.25f,
			-0.045f, glm::vec3(0.75f, -1.75f, 0.0f));
	}
	for (size_t i = 0; i < move_list_size + 2; ++i)
	{
		m_state.ui[i].render(program);
	}
}