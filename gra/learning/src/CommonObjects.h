#pragma once

#ifndef COMOON_OBJECTS_H
#define COMOON_OBJECTS_H

#include "Object.h"
#include <math.h>

Mesh *make_cube_mesh(std::string name, glm::vec3 color, float size);

std::unique_ptr<Object> make_cube(
    std::string name,
    std::unique_ptr<MaterialManager> &mat_manager,
    glm::vec3 color,
    glm::vec3 position,
    glm::vec3 rotation,
    glm::vec3 scale);

std::unique_ptr<Object> make_pig(
    std::string name,
    std::unique_ptr<MaterialManager> &mat_manager,
    glm::vec3 color,
    glm::vec3 position,
    glm::vec3 rotation,
    glm::vec3 scale);

class PigObject : public Object
{
private:
    glm::vec3 start_pos;
    glm::vec3 end_pos;

    enum State
    {
        JUMP_START,
        DOWN_START,
        MOVE_END,
        JUMP_END,
        DOWN_END,
        ROTATE_END,
        MOVE_START,
        R_START
    };
    State state;
    float jump;

public:
    PigObject(
        std::string name,
        SharedTexture diffuse,
        SharedTexture specular,
        std::shared_ptr<Material> material) : Object(name, diffuse, specular, material),
                                              state(JUMP_START),
                                              jump(10.f)
    {
    }

    void initialize()
    {
        // Position of body is initial position
        this->start_pos = this->mesh[0]->position;
        this->end_pos = this->start_pos + glm::vec3(0.f, 0.f, -50.f);
        Object::initialize();
    }

    void update(int delta)
    {
        int jump_modifier = 0.8f;
        if (this->state == JUMP_START)
        {
            this->mesh[0]->position += glm::vec3(0.f, jump_modifier, 0.f);
        }
        update_matrices(false);
    }
};

#endif // !1 COMOON_OBJECTS_H