#pragma once

#ifndef LIGHT_H
#define LIGHT_H

#include "Shaders.h"

class Light
{
public:
  glm::vec3 position;
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;

  Light(glm::vec3 position) : position(position),
                              ambient(glm::vec3(0.1)),
                              diffuse(glm::vec3(1.f)),
                              specular(glm::vec3(1.f))
  {
  }

  void send_to_shader(Shaders *program);
};

#endif // !1 LIGHT_H
