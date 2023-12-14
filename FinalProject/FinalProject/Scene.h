// Scene.h
#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Utility.h"
#include "Entity.h"
#include "Map.h"
#include "UI.h"

struct GameState
{
    Map* map;
    Entity* player;
    Entity* opp_monsters;
    UI* ui;
};

class Scene {
public:
    GameState m_state;

    // used only for buttons
    // button code is in SCENE class instead of the UI class because
    // we don't want buttons to be responsible for other buttons
    // probably a cleaner implementation to do this however
    int button_index = 0;
    int button_count = 0;
    void const set_button_count(int new_button_count) { button_count = new_button_count; };

    virtual void initialise() = 0;
    virtual void update(float delta_time) = 0;
    virtual void render(ShaderProgram* program) = 0;

    GameState const get_state()             const { return m_state; }
};