#include "Lose.h"

Lose::~Lose()
{
	delete[] m_state.ui;
}

void Lose::initialise()
{
	m_state.ui = new UI[1];

	// set first UI as textbox
	m_state.ui[0].set_ui_type(BUTTON);
	m_state.ui[0].set_position(glm::vec3(0.0f));
	m_state.ui[0].set_width(4.0f);
	m_state.ui[0].set_height(4.0f);
	m_state.ui[0].m_texture_id = Utility::load_texture("LoseScreen.png");
}

void Lose::update(float delta_time)
{
	for (size_t i = 0; i < 1; ++i)
	{
		m_state.ui[i].update(delta_time);
	}
}

void Lose::render(ShaderProgram* program)
{
	for (size_t i = 0; i < 1; ++i)
	{
		m_state.ui[i].render(program);
	}

	Utility::draw_text(program, Utility::load_texture("font.png"), "The monsters got you.", 0.25f,
		-0.045f, glm::vec3(-3.0f, -1.75f, 0.0f));
}