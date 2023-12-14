#include "Overworld.h"

// add filepath to player spritesheet later
const char PLAYER_FILEPATH[] = "Player-Sheet.png",
GOG_FILEPATH[] = "GogOverworld.png",
HIDEYNA_FILEPATH[] = "HideynaOverworld.png";

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
	m_state.player->set_position(glm::vec3(0.0f));
	m_state.player->set_speed(4.0f);
    m_state.player->m_texture_id = Utility::load_texture(PLAYER_FILEPATH);

    // player -- walking animation
    m_state.player->m_walking[m_state.player->LEFT] = new int[3] { 2, 6, 10 };
    m_state.player->m_walking[m_state.player->RIGHT] = new int[3] { 3, 7, 11 };
    m_state.player->m_walking[m_state.player->UP] = new int[3] { 1, 5, 9 };
    m_state.player->m_walking[m_state.player->DOWN] = new int[3] { 0, 4, 8 };

    m_state.player->m_animation_indices = m_state.player->m_walking[m_state.player->DOWN];  // start George looking left
    m_state.player->m_animation_frames = 3;
    m_state.player->m_animation_index = 0;
    m_state.player->m_animation_time = 0.0f;
    m_state.player->m_animation_cols = 4;
    m_state.player->m_animation_rows = 3;

    // enemy monster initializations
    m_state.opp_monsters = new Entity[5];
    for (int i = 0; i < 5; ++i)
    {
        m_state.opp_monsters[i].set_entity_type(OPP_MONSTER);
        m_state.opp_monsters[i].set_speed(2.0f);
        m_state.opp_monsters[i].set_width(2.0f);
        m_state.opp_monsters[i].set_height(2.0f);
    }

    m_state.opp_monsters[0].set_position(glm::vec3(0.0f, 3.0f, 0.0f));
    m_state.opp_monsters[0].m_texture_id = Utility::load_texture(GOG_FILEPATH);
    for (int i = 1; i < 5; ++i)
    {
        m_state.opp_monsters[i].m_texture_id = Utility::load_texture(HIDEYNA_FILEPATH);
        m_state.opp_monsters[i].set_position(glm::vec3(-1.0f + i, -1.0f, 0.0f));
    }
}

void Overworld::update(float delta_time)
{
	// make player collide with overworld enemies later
	m_state.player->update(delta_time, m_state.player, NULL, 0, m_state.map);
    for (int i = 0; i < 5; ++i)
    {
        m_state.opp_monsters->update(delta_time, m_state.player, m_state.player, 1, m_state.map);
    }
}

void Overworld::render(ShaderProgram* program)
{
	m_state.player->render(program);
    for (int i = 0; i < 5; ++i)
    {
        m_state.opp_monsters->render(program);
    }
}