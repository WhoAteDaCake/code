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
  float constant;
  float linear;
  float quadratic;

  Light(glm::vec3 position) : position(position),
                              ambient(glm::vec3(0.1)),
                              diffuse(glm::vec3(1.f)),
                              specular(glm::vec3(1.f)),
                              constant(1.f),
                              linear(0.09),
                              quadratic(0.032f)
  {
  }

  void send_to_shader(Shaders *program);
};

#endif // !1 LIGHT_H
