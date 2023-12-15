/**
* Author: Vitoria Tullo
* Assignment: Animalus
* Date due: 2023-12-15, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#define GL_SILENCE_DEPRECATION
#define GL_GLEXT_PROTOTYPES 1
#define FIXED_TIMESTEP 0.0166666f
#define LEVEL1_WIDTH 14
#define LEVEL1_HEIGHT 8
#define LEVEL1_LEFT_EDGE 5.0f

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <vector>
#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "cmath"
#include <ctime>
#include <vector>
#include "Entity.h"
#include "Map.h"
#include "Utility.h"
#include "UI.h"
#include "Scene.h"
#include "Opening.h"
#include "Selection.h"
#include "Overworld.h"
#include "Battle.h"
#include "Win.h"
#include "Lose.h"

// CONSTS
// window dimensions + viewport
const int WINDOW_WIDTH = 640 * 2,
WINDOW_HEIGHT = 480 * 2;

const int VIEWPORT_X = 0,
VIEWPORT_Y = 0,
VIEWPORT_WIDTH = WINDOW_WIDTH,
VIEWPORT_HEIGHT = WINDOW_HEIGHT;

// background color -- CHANGE LATER
const float BG_RED = 0.337f,
BG_BLUE = 0.376f,
BG_GREEN = 0.839f,
BG_OPACITY = 1.0f;

// shaders
const char V_SHADER_PATH[] = "shaders/vertex_textured.glsl",
F_SHADER_PATH[] = "shaders/fragment_textured.glsl";

const float MILLISECONDS_IN_SECOND = 1000.0;

const int CD_QUAL_FREQ = 44100,  // compact disk (CD) quality frequency
AUDIO_CHAN_AMT = 2,
AUDIO_BUFF_SIZE = 4096;

const int PLAY_ONCE = 0,
NEXT_CHNL = -1,  // next available channel
MUTE_VOL = 0,
MILS_IN_SEC = 1000,
ALL_SFX_CHN = -1;

Mix_Chunk* g_button_sound;

// ————— GLOBAL VARIABLES ————— //
Scene* g_current_scene;
Opening* g_opening;
Selection* g_selection;
Overworld* g_overworld;
Battle* g_battle;
Win* g_win;
Lose* g_lose;

SDL_Window* g_display_window;
bool g_game_is_running = true;

ShaderProgram g_shader_program;
glm::mat4 g_view_matrix, g_projection_matrix;

float g_previous_ticks = 0.0f;
float g_accumulator = 0.0f;

std::vector<Monster*> party;
std::vector<bool> monsters_status = {true, true, true, true, true};
int monster_battling = 0;

// needed to buffer input when going from the opening to the selection screen
float selection_delay = 2.0f;
bool can_select = false;

/*
* Switches to the scene that is passed in
* 
* @param scene, a SCENE object that is passed in and initialised
* Also set at the current scene
*/
void switch_to_scene(Scene* scene)
{
    g_current_scene = scene;
    g_current_scene->initialise();
}

/*
* Initialises everything in main such as sound logic,
* the window, and all the scenes to be used
* Runs once in the beginning of the game
*/
void initialise()
{
    // ————— VIDEO ————— //
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    g_display_window = SDL_CreateWindow("FINAL PROJECT :(",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_OPENGL);

    Mix_OpenAudio(
        CD_QUAL_FREQ,        // the frequency to playback audio at (in Hz)
        MIX_DEFAULT_FORMAT,  // audio format
        AUDIO_CHAN_AMT,      // number of channels (1 is mono, 2 is stereo, etc).
        AUDIO_BUFF_SIZE      // audio buffer size in sample FRAMES (total samples divided by channel count)
    );

    g_button_sound = Mix_LoadWAV("SelectSound.wav"); // button select sound

    SDL_GLContext context = SDL_GL_CreateContext(g_display_window);
    SDL_GL_MakeCurrent(g_display_window, context);

#ifdef _WINDOWS
    glewInit();
#endif

    // ————— GENERAL ————— //
    glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);

    g_shader_program.load(V_SHADER_PATH, F_SHADER_PATH);

    g_view_matrix = glm::mat4(1.0f);
    g_projection_matrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    g_shader_program.set_projection_matrix(g_projection_matrix);
    g_shader_program.set_view_matrix(g_view_matrix);

    glUseProgram(g_shader_program.get_program_id());

    glClearColor(BG_RED, BG_BLUE, BG_GREEN, BG_OPACITY);

    // levels setup
    g_opening = new Opening();
    g_selection = new Selection();
    g_overworld = new Overworld();
    g_lose = new Lose();
    g_win = new Win();
    // The BATTLE class is not here because you want to create a new BATTLE everytime you need one

    // Start at first level
    switch_to_scene(g_opening);

    // ————— BLENDING ————— //
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

/*
* Processes the input from the player
* Runs every frame
*/
void process_input()
{
    // When you are in the overworld, there is a movable player -- reset their movement
    if (g_current_scene == g_overworld) g_current_scene->m_state.player->set_movement(glm::vec3(0.0f));

    SDL_Event event;
    // check if game is quit
    while (SDL_PollEvent(&event))
    {
        switch (event.type) {
            // End game
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            g_game_is_running = false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_q:
                // Quit the game with a keystroke
                g_game_is_running = false;
                break;
            case SDLK_RETURN:
                if (g_current_scene == g_opening)
                {
                    // if there is supposed to be a textbox in a scene
                    // first UI in the scene should ALWAYS BE A TEXTBOX
                    if (g_current_scene->m_state.ui[0].get_wait_flag())
                        g_current_scene->m_state.ui[0].change_wait_flag();
                    if (!g_current_scene->m_state.ui[0].get_active_flag())
                        switch_to_scene(g_selection);
                }
                if (g_current_scene == g_selection && can_select)
                {
                    /*
                    * The selection menu has a very small buffer before you can select a monster
                    * This is because you are using enter for the dialogue text that's before this
                    * In the frame that the scene switch occurs you might still be pressing enter
                    * And accidentally select a monster
                    */
                    party.push_back(g_selection->monster_select());
                    switch_to_scene(g_overworld);
                }
                if (g_current_scene == g_battle) g_battle->player_inputted = true;
                break;
            case SDLK_a:
                if (g_current_scene == g_selection || g_current_scene == g_battle)
                {
                    // Iterate through the array of buttons in the current scene
                    // And wrap around
                    g_current_scene->button_index--;
                    if (g_current_scene->button_index < 0) g_current_scene->button_index = g_current_scene->button_count;
                    Mix_PlayChannel(
                        NEXT_CHNL,       // using the first channel that is not currently in use...
                        g_button_sound,  // ...play this chunk of audio...
                        PLAY_ONCE        // ...once.
                    );
                }
                break;
            case SDLK_d:
                if (g_current_scene == g_selection || g_current_scene == g_battle)
                {
                    // Iterate through the array of buttons in the current scene
                    // And wrap around
                    g_current_scene->button_index++;
                    if (g_current_scene->button_index > g_current_scene->button_count) g_current_scene->button_index = 0;
                    Mix_PlayChannel(
                        NEXT_CHNL,       // using the first channel that is not currently in use...
                        g_button_sound,  // ...play this chunk of audio...
                        PLAY_ONCE        // ...once.
                    );
                }
                break;
            }
        }

        const Uint8* key_state = SDL_GetKeyboardState(NULL);

        // PLAYER INPUT FOR THE OVERWORLD
        // Controls movement direction and the animation
        if (g_current_scene == g_overworld)
        {
            if (key_state[SDL_SCANCODE_A])
            {
                g_current_scene->m_state.player->move_left();
                g_current_scene->m_state.player->m_animation_indices = g_current_scene->m_state.player->m_walking[g_current_scene->m_state.player->LEFT];
            }
            else if (key_state[SDL_SCANCODE_D])
            {
                g_current_scene->m_state.player->move_right();
                g_current_scene->m_state.player->m_animation_indices = g_current_scene->m_state.player->m_walking[g_current_scene->m_state.player->RIGHT];
            }
            else if (key_state[SDL_SCANCODE_W])
            {
                g_current_scene->m_state.player->move_up();
                g_current_scene->m_state.player->m_animation_indices = g_current_scene->m_state.player->m_walking[g_current_scene->m_state.player->UP];
            }
            else if (key_state[SDL_SCANCODE_S])
            {
                g_current_scene->m_state.player->move_down();
                g_current_scene->m_state.player->m_animation_indices = g_current_scene->m_state.player->m_walking[g_current_scene->m_state.player->DOWN];
            }
            else if (key_state[SDL_SCANCODE_H] && key_state[SDL_SCANCODE_LSHIFT])
            {
                // debug cheat
                party[0]->change_health(1000);
            }

            // ————— NORMALISATION ————— //
            if (glm::length(g_current_scene->m_state.player->get_movement()) > 1.0f)
            {
                g_current_scene->m_state.player->set_movement(glm::normalize(g_current_scene->m_state.player->get_movement()));
            }
        }
    }
}

/*
* Updates all objects in the game including entities
* Also keeps an accurate track of time known as delta time
* Is called every frame
*/
void update()
{
    // ————— DELTA TIME / FIXED TIME STEP CALCULATION ————— //
    float ticks = (float)SDL_GetTicks() / MILLISECONDS_IN_SECOND;
    float delta_time = ticks - g_previous_ticks;
    g_previous_ticks = ticks;

    delta_time += g_accumulator;

    if (delta_time < FIXED_TIMESTEP)
    {
        g_accumulator = delta_time;
        return;
    }

    while (delta_time >= FIXED_TIMESTEP) {
        // ————— UPDATING THE SCENE (i.e. map, character, enemies...) ————— //
        g_current_scene->update(FIXED_TIMESTEP);

        delta_time -= FIXED_TIMESTEP;
    }

    // Timer for the input buffer on the selection screen
    if (g_current_scene == g_selection)
    {
        if (!can_select) selection_delay -= delta_time;
        if (selection_delay <= 0.0f) can_select = true;
    }

    // If the boss monster is dead, then you win the game
    if (!monsters_status[0])
    {
        switch_to_scene(g_win);
    }

    // All overworld logic that needs main's information
    if (g_current_scene == g_overworld)
    {
        // Makes sure the player doesn't continue their animation when not moving
        if (glm::length(g_current_scene->m_state.player->get_movement()) != 0)
        {
            g_current_scene->m_state.player->m_animation_time += delta_time;
            float frames_per_second = (float)1 / 30;

            // Handles animation playback
            if (g_current_scene->m_state.player->m_animation_time >= frames_per_second)
            {
                g_current_scene->m_state.player->m_animation_time = 0.0f;
                g_current_scene->m_state.player->m_animation_index++;

                if (g_current_scene->m_state.player->m_animation_index >= g_current_scene->m_state.player->m_animation_frames)
                {
                    g_current_scene->m_state.player->m_animation_index = 0;
                }
            }
        }

        // Camera follows player's exact position
        g_view_matrix = glm::mat4(1.0f);
        g_view_matrix = glm::translate(g_view_matrix, glm::vec3(-g_current_scene->m_state.player->get_position().x,
            -g_current_scene->m_state.player->get_position().y, 0));

        // This probably shouldn't be hard-coded...
        for (size_t i = 0; i < 5; ++i)
        {
            // Make sure there is monster initialzed before checking
            // if they want to start a battle
            if (g_current_scene->m_state.opp_monsters)
            {
                // Delete a monster from the overworld if they are killed
                if (!monsters_status[i]) g_current_scene->m_state.opp_monsters[i].disable();
                if (g_current_scene->m_state.opp_monsters[i].start_battle)
                {
                    monster_battling = i;
                    // Camera position stays the same even when switching scenes
                    // Undo the player follow to reset the camera
                    g_view_matrix = glm::translate(g_view_matrix, glm::vec3(g_current_scene->m_state.player->get_position().x,
                        g_current_scene->m_state.player->get_position().y, 0));
                    Battle* new_battle = new Battle();
                    new_battle->set_battle_monsters(party[0], g_current_scene->m_state.opp_monsters[i].get_monster_obj());
                    g_battle = new_battle;
                    switch_to_scene(g_battle);
                }
            }
        }
    }

    // Battle switch scene logic
    if (g_current_scene == g_battle)
    {
        /*
        * If the player's monster is dead by the end of the battle, they lose
        * If the opponent monster is dead then change their death flag in main
        * and switch back to the overworld
        */
        if (g_battle->battle_ended)
        {
            if (g_battle->get_player_monster()->get_health() == 0) switch_to_scene(g_lose);
            if (g_battle->get_opponent_monster()->get_health() == 0) monsters_status[monster_battling] = false;
            if (g_battle->get_player_monster()->get_health() != 0) switch_to_scene(g_overworld);
        }
    }
}

/*
* Renders and displays all objects in the game
* Is called every frame
*/
void render()
{
    g_shader_program.set_view_matrix(g_view_matrix);

    glClear(GL_COLOR_BUFFER_BIT);

    // ————— RENDERING THE SCENE (i.e. map, character, enemies...) ————— //
    g_current_scene->render(&g_shader_program);
    SDL_GL_SwapWindow(g_display_window);
}

/*
* Makes sure to shutdown safely
*/
void shutdown()
{
    SDL_Quit();

    // ————— DELETING LEVEL A DATA (i.e. map, character, enemies...) ————— //
    delete g_opening;
}

// ————— GAME LOOP ————— //
int main(int argc, char* argv[])
{
    initialise();

    while (g_game_is_running)
    {
        process_input();
        update();
        render();
    }

    shutdown();
    return 0;
}