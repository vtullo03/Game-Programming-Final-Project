
#define GL_SILENCE_DEPRECATION
#define STB_IMAGE_IMPLEMENTATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <fstream>
#include <vector>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "UI.h"

// credit to ninjikin on itch.io
const char FONT_FILEPATH[] = "font.png";

// --- TEXTBOX LOGIC --- 

void const UI::simulate_type_delay(float delta_time)
{
	text_timer -= delta_time;
	if (text_timer <= 0.0f)
	{
		can_print_next = true;
		text_timer = max_text_time;
	}
}

void const UI::textbox_manager(float delta_time)
{
	switch (m_textbox_state)
	{
	case (INIT):
		load_text();
		m_textbox_state = LOADTEXT;
		break;
	case (LOADTEXT):
		typewrite_text(delta_time);
		break;
	case (WAITING):
		if (!m_waiting_for_next)
		{
			m_text_index++;
			m_current_text.clear();
			m_textbox_state = LOADTEXT;
		}
		break;
	}
}

void const UI::load_text()
{
	// open some text file -- holds all dialogue for text boxes
	std::ifstream text_file;
	text_file.open(m_text_file_input);

	// if text file exists
	if (text_file)
	{
		// load it up
		std::string segment;
		char current_char;

		int counter = 0;
		while (text_file >> current_char)
		{
			// text box maximum is the max chars we can fit in any given text box 
			// before it looks strange
			segment += current_char;
			counter++;
			if (counter == m_textbox_max)
			{
				counter = 0;
				m_text_segments.push_back(segment);
			}
		}
		if (counter != 0) { m_text_segments.push_back(segment); } // get left text but don't repeat
	}
	else
	{
		std::cerr << "Textbox file could not be found!\n";
	}

	// close when done
	text_file.close();
}

void const UI::typewrite_text(float delta_time)
{
	// end of message -- don't run & hide textbox
	if (m_text_index == m_text_segments.size() - 1) disable();
	simulate_type_delay(delta_time); // add a pause between characters
	if (can_print_next)
	{
		m_current_text += m_text_segments[m_text_index][m_text_segment_index];
		m_text_segment_index++;
	}
	if (m_text_segment_index == m_text_segments[m_text_index].size())
	{
		m_waiting_for_next = true;
		m_textbox_state = WAITING;
	}
}

void UI::update(float delta_time)
{
	// if not active -- then can't update, treat like deletion
	if (!m_is_active) return;

	if (m_ui_type == TEXTBOX) textbox_manager(delta_time);
}

void UI::render(ShaderProgram * program)
{
	// if not active -- then can't render, treat like deletion
	if (!m_is_active) { return; }
	if (m_ui_type == TEXTBOX) Utility::draw_text(program, Utility::load_texture(FONT_FILEPATH), m_current_text, 0.25f,
		-0.045f, m_position);
	else
	{
		float vertices[] = {-0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5};
		float tex_coords[] = { 0.0,  1.0, 1.0,  1.0, 1.0, 0.0,  0.0,  1.0, 1.0, 0.0,  0.0, 0.0 };

		glVertexAttribPointer(program->get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
		glEnableVertexAttribArray(program->get_position_attribute());
		glVertexAttribPointer(program->get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, tex_coords);
		glEnableVertexAttribArray(program->get_tex_coordinate_attribute());

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableVertexAttribArray(program->get_position_attribute());
		glDisableVertexAttribArray(program->get_tex_coordinate_attribute());
	}
}