#include "Opening.h"
#include "Utility.h"

Opening::~Opening()
{
	delete m_state.map;
	delete m_state.player;
	delete[] m_state.ui;
}

void Opening::initialise()
{
	m_state.ui = new UI[1];
	for (size_t i = 0; i < 1; ++i)
	{
		m_state.ui[i].set_ui_type(TEXTBOX);
		m_state.ui[i].set_position(glm::vec3(0.0f));
		m_state.ui[i].set_text_file_input("Test.txt");
		m_state.ui[i].set_textbox_max(5);
	}
}

void Opening::update(float delta_time)
{
	for (size_t i = 0; i < 1; ++i)
	{
		m_state.ui[i].update(delta_time);
	}
}

void Opening::render(ShaderProgram* program)
{
	for (size_t i = 0; i < 1; ++i)
	{
		m_state.ui[i].render(program);
	}
}