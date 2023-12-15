
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

const char FONT_FILEPATH[] = "font.png";

// --- TEXTBOX LOGIC --- 

/*
* Waits for the UI object's max text time before setting
* The can print flag to true
* This creates a pause before each char that is typed out
* in textboxes
* 
* @param delta_time, an accurate measure of time so the delay is accurate
*/
void const UI::simulate_type_delay(float delta_time)
{
	text_timer -= delta_time;
	if (text_timer <= 0.0f)
	{
		can_print_next = true;
		text_timer = max_text_time;
	}
}

/*
* Manages the states of the textbox
* 
* The STATES:
* INIT -- Calls load_text func and prepares all the text that needs to be displayed
* LOADTEXT -- After init, we load the **current** text that is going to be shown to
* the player
* WAITING -- Waits for the player's input before moving onto the next text
* Only move on if there IS text to be displayed and clear the current text before moving on
* 
* @param delta_time, an accurate measure of time so the text delay is accurate
*/
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

/*
* Loads the CURRENT TEXT that needs to be displayed to the player
*/
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
		while (text_file >> std::noskipws >> current_char)
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

/*
* Display each character in the current text to be displayed with a delay
* 
* @param delta_time, an accurate measure of time so the delay is accurate
*/
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

/*
* The GROWTH effect used by buttons when you select them
*/
void const UI::button_change_size()
{
	if (button_is_selected) m_model_matrix = glm::scale(m_model_matrix, glm::vec3(1.1f, 1.1f, 1.0f));
	else if (!button_is_selected) m_model_matrix = glm::scale(m_model_matrix, glm::vec3(1.0f, 1.0f, 1.0f));
}

/*
* Update all UI logic -- called every frame
* 
* @param delta_time, used by several UI funcs as an accurate measurement of time
*/
void UI::update(float delta_time)
{
	// if not active -- then can't update, treat like deletion
	if (!m_is_active) return;

	if (m_ui_type == TEXTBOX) textbox_manager(delta_time);
	else
	{
		// reset model before every change
		m_model_matrix = glm::mat4(1.0f);
		m_model_matrix = glm::translate(m_model_matrix, m_position);
		m_model_matrix = glm::scale(m_model_matrix, glm::vec3(m_width, m_height, 1.0f));
	}
}

/*
* Render all UI elements -- called every frame
*/
void UI::render(ShaderProgram * program)
{
	// if not active -- then can't render, treat like deletion
	if (!m_is_active) { return; }
	if (m_ui_type == TEXTBOX) Utility::draw_text(program, Utility::load_texture(FONT_FILEPATH), m_current_text, 0.25f,
		-0.045f, m_position);
	else
	{
		program->set_model_matrix(m_model_matrix);

		// if not active -- then can't render, treat like deletion
		if (!m_is_active) { return; }

		float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
		float tex_coords[] = { 0.0,  1.0, 1.0,  1.0, 1.0, 0.0,  0.0,  1.0, 1.0, 0.0,  0.0, 0.0 };

		glBindTexture(GL_TEXTURE_2D, m_texture_id);

		glVertexAttribPointer(program->get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
		glEnableVertexAttribArray(program->get_position_attribute());
		glVertexAttribPointer(program->get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, tex_coords);
		glEnableVertexAttribArray(program->get_tex_coordinate_attribute());

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableVertexAttribArray(program->get_position_attribute());
		glDisableVertexAttribArray(program->get_tex_coordinate_attribute());
	}
}