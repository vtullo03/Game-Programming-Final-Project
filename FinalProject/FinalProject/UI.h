#pragma once

// for different types of UI
enum UIType { TEXTBOX, BUTTON };
enum TextboxState { INIT, LOADTEXT, WAITING };

#include "Utility.h"

class UI
{
private:
	// Transformation constraints -- used by all UI
	glm::vec3 m_position;
	glm::mat4 m_model_matrix;
	float m_width = 1.0f;
	float m_height = 1.0f;

	// Properties
	bool m_is_active = true;
	UIType m_ui_type;

	// Used by textboxes
	TextboxState m_textbox_state;
	bool m_waiting_for_next = false;
	bool can_print_next = false;
	int m_text_segment_index = 0;
	int m_text_index = 0;
	int m_textbox_max = 0;
	float const max_text_time = 2.0f;
	float text_timer = max_text_time;
	std::vector<std::string> m_text_segments;
	std::string m_text_file_input; // all long dialog/textboxes will use .txt files
	std::string m_current_text;

	// Used by buttons
	bool button_is_selected = false;

public:
	GLuint m_texture_id; // texture -- used by buttons

	// constructor and deconstructor

	// game functions
	void update(float delta_time);
	void render(ShaderProgram* program);

	// change flags
	void const change_wait_flag() { m_waiting_for_next = !m_waiting_for_next; }
	void const activiate() { m_is_active = true; }
	void const disable() { m_is_active = false; }

	// setters and getters - for all
	void const set_position(glm::vec3 new_position) { m_position = new_position; };
	void const set_ui_type(UIType new_ui_type) { m_ui_type = new_ui_type; };
	void const set_width(float new_width) { m_width = new_width; };
	void const set_height(float new_height) { m_height = new_height; };

	// flags - textbox
	bool const get_wait_flag() const { return m_waiting_for_next; };
	bool const get_active_flag() const { return m_is_active; };
	// setters and getters - textbox 
	std::string get_text_file_input() const { return m_text_file_input;}
	void const set_text_file_input(std::string new_text_file_input) { m_text_file_input = new_text_file_input; };
	void const set_textbox_max(int new_textbox_max) { m_textbox_max = new_textbox_max; };

	// flags - button
	void const button_selected() { button_is_selected = true; }
	void const button_unselected() { button_is_selected = false; }

	// textbox logic
	void const start_textbox() { m_textbox_state = INIT; }
	void const textbox_manager(float delta_time);
	void const load_text();
	void const simulate_type_delay(float delta_time);
	void const typewrite_text(float delta_time);

	// button logic
	void const button_change_size();
};