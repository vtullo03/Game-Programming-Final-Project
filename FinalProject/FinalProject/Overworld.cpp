#include "Overworld.h"

#define OVERWORLD_WIDTH 14
#define OVERWORLD_HEIGHT 12

// add filepath to player spritesheet later
const char PLAYER_FILEPATH[] = "Player-Sheet.png",
GOG_FILEPATH[] = "GogOverworld.png",
HIDEYNA_FILEPATH[] = "HideynaOverworld.png",
MAP_FILEPATH[] = "RockyWallsTileset.png";

unsigned int OVERWORLD_DATA[] =
{
    3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1,
    3, 0, 0, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 1,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1,
    3, 2, 2, 2, 2, 2, 0, 0, 0, 1, 0, 0, 0, 1,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1
};

Overworld::~Overworld()
{
    delete m_state.map;
    delete m_state.player;
    delete m_state.opp_monsters;
    delete m_state.ui;
}

void Overworld::initialise()
{
    GLuint map_texture_id = Utility::load_texture(MAP_FILEPATH);
    m_state.map = new Map(OVERWORLD_WIDTH, OVERWORLD_HEIGHT, OVERWORLD_DATA, map_texture_id, 1.0f, 3, 1);

	m_state.player = new Entity();
	m_state.player->set_entity_type(PLAYER);
	m_state.player->set_position(glm::vec3(12.0f, -1.0f, 0.0f));
	m_state.player->set_speed(4.0f);
    m_state.player->m_texture_id = Utility::load_texture(PLAYER_FILEPATH);

    // player -- walking animation
    m_state.player->m_walking[m_state.player->LEFT] = new int[3] { 2, 6, 10 };
    m_state.player->m_walking[m_state.player->RIGHT] = new int[3] { 3, 7, 11 };
    m_state.player->m_walking[m_state.player->UP] = new int[3] { 1, 5, 9 };
    m_state.player->m_walking[m_state.player->DOWN] = new int[3] { 0, 4, 8 };

    m_state.player->m_animation_indices = m_state.player->m_walking[m_state.player->DOWN];
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

    m_state.opp_monsters[0].set_position(glm::vec3(8.0f, 0.0f, 0.0f));
    m_state.opp_monsters[0].m_texture_id = Utility::load_texture(GOG_FILEPATH);
    Move* primary_move = new Move("Blizzard", 0, 10, OPP_SPEED_CHANGE);
    Move* secondary_move = new Move("Avalanche", 20, 0, OPP_HEALTH_CHANGE);
    m_state.opp_monsters[0].set_monster_obj(new Monster("GogLordofTheMountain",
        100, 10, std::vector<Move*>{primary_move, secondary_move}));
    m_state.opp_monsters[0].set_monster_type(BOSS);

    for (int i = 1; i < 5; ++i)
    {
        m_state.opp_monsters[i].m_texture_id = Utility::load_texture(HIDEYNA_FILEPATH);
        Move* primary_move = new Move("Camouflage", 0, 5, SPEED_CHANGE);
        Move* secondary_move = new Move("Bite", 10, 0, OPP_HEALTH_CHANGE);
        m_state.opp_monsters[i].set_monster_obj(new Monster("Hideyna",
            20, 10, std::vector<Move*>{primary_move, secondary_move}));
        m_state.opp_monsters[i].set_monster_type(LITTLEGUY);
    }

    m_state.opp_monsters[1].set_position(glm::vec3(11.0f, -3.0f, 0.0f));
    m_state.opp_monsters[2].set_position(glm::vec3(11.0f, -6.0f, 0.0f));
    m_state.opp_monsters[3].set_position(glm::vec3(8.0f, -7.0f, 0.0f));
    m_state.opp_monsters[4].set_position(glm::vec3(1.0f, -3.0f, 0.0f));
}

void Overworld::update(float delta_time)
{
	// make player collide with overworld enemies later
	m_state.player->update(delta_time, m_state.player, NULL, 0, m_state.map);
    for (int i = 0; i < 5; ++i)
    {
        m_state.opp_monsters[i].update(delta_time, m_state.player, m_state.player, 1, m_state.map);
    }
}

void Overworld::render(ShaderProgram* program)
{
	m_state.player->render(program);
    m_state.map->render(program);
    for (int i = 0; i < 5; ++i)
    {
        m_state.opp_monsters[i].render(program);
    }
}