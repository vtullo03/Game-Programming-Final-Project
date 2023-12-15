#include "Opening.h"
#include "Utility.h"

const char BGM_FILEPATH[] = "Disquiet.mp3";
const int    LOOP_FOREVER = -1;  // -1 means loop forever in Mix_PlayMusic; 0 means play once and loop zero times

Mix_Music* g_music;

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

	g_music = Mix_LoadMUS(BGM_FILEPATH);
	Mix_PlayMusic(g_music, LOOP_FOREVER);
	Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
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