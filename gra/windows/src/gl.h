#pragma once

#ifndef GL_H
#define GL_H

#define GLM_ENABLE_EXPERIMENTAL

// Usually defined in the compiler
// #define GRA_DEBUG

#ifdef __linux__
#include <GL/freeglut.h>
#else
#include "glad.h"
#include <GL/freeglut.h>
#endif


#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/normal.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/color_space.hpp>
#include <glm/gtx/component_wise.hpp>

#endif
