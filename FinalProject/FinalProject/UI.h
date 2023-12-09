#pragma once

// for different types of UI
enum UIType { TEXTBOX, PANEL, IMAGE };
enum TextboxState { INIT, LOADTEXT, WAITING };

#include "Utility.h"

class UI
{
private:
	// Transformation constraints -- used by all UI
	glm::vec3 m_position;
	float m_width = 1.0f;
	float m_height = 1.0f;

	// Properties
	bool m_is_active = true;
	UIType m_ui_type;

	// Used by textboxes
	TextboxState m_textbox_state;
	bool m_waiting_for_next = false;
	std::vector<std::string> m_text_segments;
	std::string m_text_file_input; // all long dialog/textboxes will use .txt files
	std::string m_current_text;
	int m_text_index = 0;
	int m_textbox_max = 0;
public:

	// constructor and deconstructor
	//UI();
	//~UI();

	// game functions
	void update(float delta_time);
	void render(ShaderProgram* program);

	// change flags
	void const change_wait_flag() { m_waiting_for_next = !m_waiting_for_next; }
	void const activiate() { m_is_active = true; }
	void const disable() { m_is_active = false; }

	// setters and getters
	// for all
	void const set_position(glm::vec3 new_position) { m_position = new_position; };
	void const set_ui_type(UIType new_ui_type) { m_ui_type = new_ui_type; };
	// flags
	bool const get_wait_flag() const { return m_waiting_for_next; };
	bool const get_active_flag() const { return m_is_active; };
	// textbox
	std::string get_text_file_input() const { return m_text_file_input;}
	void const set_text_file_input(std::string new_text_file_input) { m_text_file_input = new_text_file_input; };
	void const set_textbox_max(int new_textbox_max) { m_textbox_max = new_textbox_max; };

	// textbox logic
	void const textbox_manager();
	void const load_text();
	void const typewrite_text();
};