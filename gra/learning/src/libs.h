#include <GL/freeglut.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <fstream>
#include <cstdio>
#include "SOIL.h"

struct Vertex
{
  glm::vec3 position;
  glm::vec3 color;
  glm::vec2 texcoord;
  // glm::vec3 normal;
};