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
        JUMP,
        DOWN,
        MOVE,
        ROTATE,
    };
    enum Position
    {
        START,
        END
    };
    State state;
    Position position;
    Position leg_position;
    float jump;
    float walk;
    float y_velocity;
    float y_velocity_max;
    // Animation bounds
    float y_end;
    float z_end;

public:
    PigObject(
        std::string name,
        SharedTexture diffuse,
        SharedTexture specular,
        std::shared_ptr<Material> material) : Object(name, diffuse, specular, material),
                                              state(JUMP),
                                              position(START),
                                              jump(15.f),
                                              walk(30.f),
                                              y_end(0.f),
                                              z_end(0.f),
                                              y_velocity(0.f),
                                              y_velocity_max(0.8f),
                                              leg_position(START)
    {
    }

    void initialize();
    void update(int delta);
};

#endif // !1 COMOON_OBJECTS_H