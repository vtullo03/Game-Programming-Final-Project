#pragma once

enum EntityType { PLAYER };

#include "Map.h"

class Entity {
private:
    // ––––– ANIMATION ––––– //
    int* m_animation_right = NULL, // move to the right
        * m_animation_left = NULL, // move to the left
        * m_animation_up = NULL, // move upwards
        * m_animation_down = NULL; // move downwards

    // position and tranformation variables
    glm::vec3 m_position;
    glm::mat4 m_model_matrix;

    // physics variables
    glm::vec3 m_velocity;
    glm::vec3 m_acceleration;
    glm::vec3 m_movement;

    // specific to different entity types
    float m_width = 1.0f;
    float m_height = 1.0f;
    float m_speed = 0.0f;

    bool m_is_active = true; // objects that are not active -- basically deleted
    bool m_is_rendered = true; // objects that are not rendered are still active

    EntityType m_entity_type; // treat as object type not name this time

public:
    // ————— STATIC VARIABLES ————— //
    static const int    SECONDS_PER_FRAME = 4;
    static const int    LEFT = 0,
        RIGHT = 1,
        UP = 2,
        DOWN = 3;

    // ————— ANIMATION ————— //
    int** m_walking = new int* [4]
    {
        m_animation_left,
            m_animation_right,
            m_animation_up,
            m_animation_down
    };

    int m_animation_frames = 0,
        m_animation_index = 0,
        m_animation_cols = 0,
        m_animation_rows = 0;

    int* m_animation_indices = NULL;
    float   m_animation_time = 0.0f;

    GLuint m_texture_id; // texture

    // default constructor
    Entity();
    //~Entity();

    void draw_sprite_from_texture_atlas(ShaderProgram* program, GLuint texture_id, int index);
    void update(float delta_time, Entity* player, Entity* objects, int object_count, Map* map);
    void render(ShaderProgram* program);

    // collisions - both in the x and y axis
    bool const check_collision(Entity* other) const;
    void const check_collision_y(Entity* collidable_entities, int collidable_entity_count);
    void const check_collision_y(Map* map);
    void const check_collision_x(Entity* collidable_entities, int collidable_entity_count);
    void const check_collision_x(Map* map);

    void activate() { m_is_active = true; };
    void deactivate() { m_is_active = false; };

    // movement
    void move_left() { m_movement.x = -1.0f; };
    void move_right() { m_movement.x = 1.0f; };
    void move_up() { m_movement.y = 1.0f; };
    void move_down() { m_movement.y = -1.0f; };

    // GETTERS
    EntityType const get_entity_type()    const { return m_entity_type; };
    glm::vec3  const get_position()       const { return m_position; };
    glm::vec3  const get_movement()       const { return m_movement; };
    glm::vec3  const get_velocity()       const { return m_velocity; };
    glm::vec3  const get_acceleration()   const { return m_acceleration; };
    float        const get_width()          const { return m_width; };
    float        const get_height()         const { return m_height; };
    float      const get_speed()          const { return m_speed; };
    bool       const get_active_state()   const { return m_is_active; };

    // SETTLERS
    void const set_entity_type(EntityType new_entity_type) { m_entity_type = new_entity_type; };
    void const set_position(glm::vec3 new_position) { m_position = new_position; };
    void const set_movement(glm::vec3 new_movement) { m_movement = new_movement; };
    void const set_velocity(glm::vec3 new_velocity) { m_velocity = new_velocity; };
    void const set_acceleration(glm::vec3 new_acceleration) { m_acceleration = new_acceleration; };
    void const set_width(float new_width) { m_width = new_width; };
    void const set_height(float new_height) { m_height = new_height; };
    void const set_speed(float new_speed) { m_speed = new_speed; };

    void const disable() { m_is_active = false; };
    void const enable() { m_is_active = true; };
};