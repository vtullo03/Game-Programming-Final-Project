
#define GL_SILENCE_DEPRECATION
#define STB_IMAGE_IMPLEMENTATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Entity.h"


/*
    The ENTITY Class's Default constructor
*/
Entity::Entity()
{
    // position and tranformation variables
    m_position = glm::vec3(0.0f);
    m_model_matrix = glm::mat4(1.0f);

    // physics variables
    m_velocity = glm::vec3(0.0f);
    m_acceleration = glm::vec3(0.0f);
    m_movement = glm::vec3(0.0f);
}

/*
* Update function specifically for the ENTITY class
* Checks collision in all cardinal directions
* Then calculates physics
* Then updates transformations
*
* @param delta_time, float that's the value of real-life time in seconds
* @param player, the player ENTITY -- mainly used by enemies
* @param objects, an array of entities that this ENTITY can collide with
* @param object_count, size of the array mentioned above
* @param map, the level's MAP object that the entity can collide with
*/
void Entity::update(float delta_time, Entity* player, Entity* objects, int object_count, Map* map)
{
    // if not active -- then can't update, treat like deletion
    if (!m_is_active) return;

    m_velocity.x = m_movement.x * get_speed();

    // must be calculated seperatedly for seperate collisions
    m_position.x += m_velocity.x * delta_time;

    m_position.y += m_velocity.y * delta_time;

    // reset model before every change
    m_model_matrix = glm::mat4(1.0f);
    m_model_matrix = glm::translate(m_model_matrix, m_position);
}

/*
* Render function specifically for the ENTITY class
*
* @param program, reference to the SHADERPROGRAM class -- to use it's functions
*/
void Entity::render(ShaderProgram* program)
{
    program->set_model_matrix(m_model_matrix);

    // if not active -- then can't render, treat like deletion
    if (!m_is_active || !m_is_rendered) { return; }

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