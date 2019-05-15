#pragma once

#ifndef COMOON_OBJECTS_H
#define COMOON_OBJECTS_H

#include "ObjectLight.h"
#include "Shapes.h"
#include <math.h>

/**
 * Creates a very simple cube mesh
 */
Mesh *make_cube_mesh(std::string name, glm::vec3 color, float size);

/**
 * Wraps a cube mesh into an object
 */
std::unique_ptr<Object> make_cube(
    std::string name,
    std::unique_ptr<MaterialManager> &mat_manager,
    glm::vec3 color,
    glm::vec3 position,
    glm::vec3 rotation,
    glm::vec3 scale);
/**
 * Creates a more complex pig object
 */
std::unique_ptr<Object> make_pig(
    std::string name,
    std::unique_ptr<MaterialManager> &mat_manager,
    glm::vec3 color,
    glm::vec3 position,
    glm::vec3 rotation,
    glm::vec3 scale);
/**
 * Creates a object that is tied to a light
 */
std::unique_ptr<ObjectLight> make_light_cube(
    std::string name,
    std::unique_ptr<MaterialManager> &mat_manager,
    glm::vec3 color,
    glm::vec3 position,
    glm::vec3 rotation,
    glm::vec3 scale);

/**
 * Creates a more complex light object, with attached spotlight
 */
std::unique_ptr<ObjectLight> make_fan_lamp(
    std::string name,
    std::unique_ptr<MaterialManager> &mat_manager,
    glm::vec3 color,
    glm::vec3 position,
    glm::vec3 rotation,
    glm::vec3 scale);

/**
 * Wrapper for Object with custom animation
 */
class PigObject : public Object
{
private:
    // Postion of initial location of the pig
    glm::vec3 start_pos;
    // Ending coordinate of the walk
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
    // How high should the jump be
    float jump;
    // The length of the walk
    float walk;
    // Gravity factors for realistic jump animation
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

/**
 * Wrapper to create a custom fan animation
 * That spins the fan blades
 */
class FanObject : public Object
{
private:
    float angle;

public:
    FanObject(
        std::string name,
        SharedTexture diffuse,
        SharedTexture specular,
        std::shared_ptr<Material> material) : Object(name, diffuse, specular, material), angle(0.f)
    {
    }

    void update(int delta);
};

#endif // !1 COMOON_OBJECTS_H