#include "Overworld.h"

// add filepath to player spritesheet later
//const char PLAYER_FILEPATH[] = "Player_Spritesheet.png"

Overworld::~Overworld()
{
	delete m_state.map;
	delete m_state.player;
	delete[] m_state.ui;
}

void Overworld::initialise()
{
	// fine tune values later
	m_state.player = new Entity();
	m_state.player->set_entity_type(PLAYER);
	m_state.player->set_position(glm:vec3(0.0f));
	m_state.player->set_speed(1.0f);
}

void Overworld::update(float delta_time)
{
	// make player collide with overworld enemies later
	m_state.player->update(delta_time, m_state.player, NULL, 0, m_state.map);
}

void Overworld::render(ShaderProgram* program)
{
	m_state.player->render(program);
}