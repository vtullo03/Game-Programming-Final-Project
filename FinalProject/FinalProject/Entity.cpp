
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
* Creates sprites from a spritesheet
* 
* @param program, the shader program that renders all the sprites
* @param texture_id, the id of the sprite sheet
* @param index, where in the sprite sheet we're trying to pull the sprite from
*/
void Entity::draw_sprite_from_texture_atlas(ShaderProgram* program, GLuint texture_id, int index)
{
    // Step 1: Calculate the UV location of the indexed frame
    float u_coord = (float)(index % m_animation_cols) / (float)m_animation_cols;
    float v_coord = (float)(index / m_animation_cols) / (float)m_animation_rows;

    // Step 2: Calculate its UV size
    float width = 1.0f / (float)m_animation_cols;
    float height = 1.0f / (float)m_animation_rows;

    // Step 3: Just as we have done before, match the texture coordinates to the vertices
    float tex_coords[] =
    {
        u_coord, v_coord + height, u_coord + width, v_coord + height, u_coord + width, v_coord,
        u_coord, v_coord + height, u_coord + width, v_coord, u_coord, v_coord
    };

    float vertices[] =
    {
        -0.5, -0.5, 0.5, -0.5,  0.5, 0.5,
        -0.5, -0.5, 0.5,  0.5, -0.5, 0.5
    };

    // Step 4: And render
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glVertexAttribPointer(program->get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->get_position_attribute());

    glVertexAttribPointer(program->get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, tex_coords);
    glEnableVertexAttribArray(program->get_tex_coordinate_attribute());

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->get_position_attribute());
    glDisableVertexAttribArray(program->get_tex_coordinate_attribute());
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
    if (monster_type == LITTLEGUY) patrol_ai();

    m_collided_top = false;
    m_collided_bottom = false;
    m_collided_left = false;
    m_collided_right = false;

    m_wallcheck_left = false;
    m_wallcheck_right = false;

    m_velocity.x = m_movement.x * get_speed();
    m_velocity.y = m_movement.y * get_speed();

    // must be calculated seperatedly for seperate collisions
    m_position.x += m_velocity.x * delta_time;
    m_position.y += m_velocity.y * delta_time;

    m_position.y += m_velocity.y * delta_time;
    check_collision_y(objects, object_count);
    check_collision_y(map);

    m_position.x += m_velocity.x * delta_time;
    check_collision_x(objects, object_count);
    check_collision_x(map);

    // reset model before every change
    m_model_matrix = glm::mat4(1.0f);
    m_model_matrix = glm::translate(m_model_matrix, m_position);
    m_model_matrix = glm::scale(m_model_matrix, glm::vec3(m_width, m_height, 1.0f));
}

/*
* Checks for collisions in the y-axis
* Iterates through all the entities that are collidable and checks if
* touching above or below.
*
* @param collidable_entities, an array of all entities that this ENTITY can collide with
* @param collidable_entity_count, size of the array above
*
* TREAT LIKE ON_COLLISION_ENTER
*/
void const Entity::check_collision_y(Entity* collidable_entities, int collidable_entity_count)
{
    for (int i = 0; i < collidable_entity_count; i++)
    {
        Entity* collidable_entity = &collidable_entities[i];

        if (check_collision(collidable_entity))
        {
            if (m_entity_type == OPP_MONSTER && collidable_entity->m_entity_type == PLAYER) start_battle = true;
            float y_distance = fabs(m_position.y - collidable_entity->get_position().y);
            float y_overlap = fabs(y_distance - (m_height / 2.0f) - (collidable_entity->get_height() / 2.0f));
            if (m_velocity.y > 0) {
                m_position.y -= y_overlap;
                m_velocity.y = 0;
                m_collided_top = true;
            }
            else if (m_velocity.y < 0) {
                m_position.y += y_overlap;
                m_velocity.y = 0;
                m_collided_bottom = true;
            }
        }
    }
}

/*
* Check for collisions with the map in the y-axis
*
* @param map, MAP object that the ENTITY object is colliding with
*/
void const Entity::check_collision_y(Map* map)
{
    // Check all tiles above, including left and right for corner interaction
    glm::vec3 top = glm::vec3(m_position.x, m_position.y + (m_height / 2), m_position.z);
    glm::vec3 top_left = glm::vec3(m_position.x - (m_width / 2), m_position.y + (m_height / 2), m_position.z);
    glm::vec3 top_right = glm::vec3(m_position.x + (m_width / 2), m_position.y + (m_height / 2), m_position.z);

    // Check all tiles belove, including left and right for corner interaction
    glm::vec3 bottom = glm::vec3(m_position.x, m_position.y - (m_height / 2), m_position.z);
    glm::vec3 bottom_left = glm::vec3(m_position.x - (m_width / 2), m_position.y - (m_height / 2), m_position.z);
    glm::vec3 bottom_right = glm::vec3(m_position.x + (m_width / 2), m_position.y - (m_height / 2), m_position.z);

    float penetration_x = 0;
    float penetration_y = 0;

    // REFACTOR LATER
    if (get_velocity().y > 0)
    {
        if (map->is_solid(top, &penetration_x, &penetration_y))
        {
            m_position.y -= penetration_y;
            m_velocity.y = 0;
            m_collided_top = true;
        }
        else if (map->is_solid(top_left, &penetration_x, &penetration_y))
        {
            if (!m_wallcheck_left && !m_wallcheck_right)
            {
                m_position.y -= penetration_y;
                m_velocity.y = 0;
                m_collided_top = true;
            }
        }
        else if (map->is_solid(top_right, &penetration_x, &penetration_y))
        {
            if (!m_wallcheck_left && !m_wallcheck_right)
            {
                m_position.y -= penetration_y;
                m_velocity.y = 0;
                m_collided_top = true;
            }
        }
    }
    else if (get_velocity().y < 0)
    {

        if (map->is_solid(bottom, &penetration_x, &penetration_y))
        {
            m_position.y += penetration_y;
            m_velocity.y = 0;
            m_collided_bottom = true;
        }
        else if (map->is_solid(bottom_left, &penetration_x, &penetration_y))
        {
            if (!m_wallcheck_left && !m_wallcheck_right)
            {
                m_position.y += penetration_y;
                m_velocity.y = 0;
                m_collided_bottom = true;
            }
        }
        else if (map->is_solid(bottom_right, &penetration_x, &penetration_y))
        {
            if (!m_wallcheck_left && !m_wallcheck_right)
            {
                m_position.y += penetration_y;
                m_velocity.y = 0;
                m_collided_bottom = true;
            }
        }
    }
}

/*
* Checks for collisions with other ENTITY objects in the x-axis
* Iterates through all the entities that are collidable and checks if
* touching to left or right.
*
* @param collidable_entities, an array of all entities that this ENTITY can collide with
* @param collidable_entity_count, size of the array above
*
* TREAT LIKE ON_COLLISION_ENTER
*/
void const Entity::check_collision_x(Entity* collidable_entities, int collidable_entity_count)
{
    for (int i = 0; i < collidable_entity_count; i++)
    {
        Entity* collidable_entity = &collidable_entities[i];

        if (check_collision(collidable_entity))
        {
            if (m_entity_type == OPP_MONSTER && collidable_entity->m_entity_type == PLAYER) start_battle = true;
            float x_distance = fabs(m_position.x - collidable_entity->get_position().x);
            float x_overlap = fabs(x_distance - (m_width / 2.0f) - (collidable_entity->get_width() / 2.0f));
            if (m_velocity.x > 0) {
                m_position.x -= x_overlap;
                m_velocity.x = 0;
                m_collided_right = true;
            }
            else if (m_velocity.x < 0) {
                m_position.x += x_overlap;
                m_velocity.x = 0;
                m_collided_left = true;
            }
        }
    }
}

/*
* Check for collisions with the map in the x-axis
*
* @param map, MAP object that the ENTITY object is colliding with
*/
void const Entity::check_collision_x(Map* map)
{
    // Check if touching tile
    glm::vec3 left = glm::vec3(m_position.x - (m_width / 2), m_position.y, m_position.z);
    glm::vec3 right = glm::vec3(m_position.x + (m_width / 2), m_position.y, m_position.z);

    // Check if touching wall
    glm::vec3 left_wall = left - glm::vec3(m_wallcheck_offset, 0.0f, 0.0f);
    glm::vec3 right_wall = right + glm::vec3(m_wallcheck_offset, 0.0f, 0.0f);

    float penetration_x = 0;
    float penetration_y = 0;

    // entity collision checks
    if (map->is_solid(left, &penetration_x, &penetration_y) && m_velocity.x < 0)
    {
        m_position.x += penetration_x;
        m_velocity.x = 0;
        m_collided_left = true;
    }
    if (map->is_solid(right, &penetration_x, &penetration_y) && m_velocity.x > 0)
    {
        m_position.x -= penetration_x;
        m_velocity.x = 0;
        m_collided_right = true;
    }

    if (map->is_solid(left_wall, &penetration_x, &penetration_y)) m_wallcheck_left = true;
    if (map->is_solid(right_wall, &penetration_x, &penetration_y)) m_wallcheck_right = true;
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

    if (m_animation_indices != NULL)
    {
        draw_sprite_from_texture_atlas(program, m_texture_id, m_animation_indices[m_animation_index]);
        return;
    }

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

/*
* General check collision function for static object
*
* @param other, the other ENTITY class that is being collided with
*
* TREAT LIKE NORMAL PHYSICS COLLISION -- STOPS OBJECTS
*/
bool const Entity::check_collision(Entity* other) const
{
    if (other == this) return false;
    // If either entity is inactive, there shouldn't be any collision
    if (!m_is_active || !other->m_is_active) return false;

    float x_distance = fabs(m_position.x - other->m_position.x) - ((m_width + other->m_width) / 2.0f);
    float y_distance = fabs(m_position.y - other->m_position.y) - ((m_height + other->m_height) / 2.0f);

    return x_distance < 0.0f && y_distance < 0.0f;
}

/*
* Patrol AI used by overworld enemies
* They move either left or right
* If they hit a wall they will about face
*/
void Entity::patrol_ai()
{
    if (is_facing_right) set_movement(glm::vec3(1.0f, 0.0f, 0.0f));
    else if (!is_facing_right) set_movement(glm::vec3(-1.0f, 0.0f, 0.0f));

    if (m_wallcheck_right) is_facing_right = false;
    if (m_wallcheck_left) is_facing_right = true;
}