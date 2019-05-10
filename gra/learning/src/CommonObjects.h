#pragma once

#ifndef COMOON_OBJECTS_H
#define COMOON_OBJECTS_H

#include "Object.h"

std::unique_ptr<Object> make_cube(
    std::string name,
    std::unique_ptr<MaterialManager> &mat_manager,
    glm::vec3 color,
    glm::vec3 position,
    glm::vec3 rotation,
    glm::vec3 scale);

#endif // !1 COMOON_OBJECTS_H