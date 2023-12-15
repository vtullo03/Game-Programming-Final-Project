#include "Opening.h"
#include "Utility.h"

Opening::~Opening()
{
	delete[] m_state.ui;
}

void Opening::initialise()
{
	m_state.ui = new UI[1];
	
	// set first UI as textbox
	m_state.ui[0].set_ui_type(TEXTBOX);
	m_state.ui[0].set_position(glm::vec3(-4.75f, -2.0f, 0.0f));
	m_state.ui[0].set_text_file_input("Opening.txt");
	m_state.ui[0].set_textbox_max(39);
	m_state.ui[0].start_textbox();
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