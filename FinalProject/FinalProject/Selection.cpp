#include "Selection.h"
#include "Utility.h"

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
	m_state.ui = new UI[3];

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
	}
}

void Selection::render(ShaderProgram* program)
{
	for (size_t i = 0; i < 3; ++i)
	{
		m_state.ui[i].render(program);
	}
}