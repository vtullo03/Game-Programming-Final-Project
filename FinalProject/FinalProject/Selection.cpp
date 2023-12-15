#include "Selection.h"

const char BUMBLEBEAR_SELECT_FILEPATH[] = "Bumblebear.png",
WHOOVES_SELECT_FILEPATH[] = "Whooves.png",
ANTELION_SELECTION_FILEPATH[] = "Antelion.png";

Selection::~Selection()
{
	delete m_state.map;
	delete m_state.player;
	delete[] m_state.ui;
}

void Selection::initialise()
{
	std::cout << "selection";
	m_state.ui = new UI[3];
	Scene::set_button_count(3);

	// set first three UIs as buttons
	for (size_t i = 0; i < 3; ++i)
	{
		m_state.ui[i].set_ui_type(BUTTON);
		m_state.ui[i].set_position(glm::vec3((-2.0f + (i*2)), 2.0f, 0.0f));
		m_state.ui[i].set_width(4.0f);
		m_state.ui[i].set_height(4.0f);
	}

	// load textures with monster images
	m_state.ui[0].m_texture_id = Utility::load_texture(BUMBLEBEAR_SELECT_FILEPATH);
	m_state.ui[1].m_texture_id = Utility::load_texture(WHOOVES_SELECT_FILEPATH);
	m_state.ui[2].m_texture_id = Utility::load_texture(ANTELION_SELECTION_FILEPATH);
}

void Selection::update(float delta_time)
{
	for (size_t i = 0; i < 3; ++i)
	{
		m_state.ui[i].update(delta_time);
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

void Selection::render(ShaderProgram* program)
{
	for (size_t i = 0; i < 3; ++i)
	{
		m_state.ui[i].render(program);
	}
}

Monster* Selection::monster_select()
{
	Move* primary_move;
	Move* secondary_move;
	switch (button_index)
	{
	case 0:
		primary_move = new Move("Bite", 10, 0, OPP_HEALTH_CHANGE);
		secondary_move = new Move("Honeysuckle", 10, 0, HEALTH_CHANGE);
		return new Monster("Bumblebear", 60, 5, std::vector<Move*>{ primary_move, secondary_move });
		break;
	case 1:
		primary_move = new Move("Stomp", 10, 0, OPP_HEALTH_CHANGE);
		secondary_move = new Move("Headbutt", 0, 5, OPP_SPEED_CHANGE);
		return new Monster("Whooves", 50, 10, std::vector<Move*>{ primary_move, secondary_move });
		break;
	case 2:
		primary_move = new Move("Claw", 10, 0, OPP_HEALTH_CHANGE);
		secondary_move = new Move("Frenzy", 0, 10, SPEED_CHANGE);
		return new Monster("Antelion", 40, 15, std::vector<Move*> { primary_move, secondary_move });
		break;
	}
}