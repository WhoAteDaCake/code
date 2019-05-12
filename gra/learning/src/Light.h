#pragma once

#ifndef LIGHT_H
#define LIGHT_H

#include "Shaders.h"

// https://learnopengl.com/Lighting/Light-casters
class Light
{
public:
  /**
   * 0 - Directional NOT_DONE
   * 1 - Point light NOT_DONE
   * 2 - Spotlight NOT_DONE
   */
  int type;

  glm::vec3 position;
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  glm::vec3 direction;
  //
  float constant;
  float linear;
  float quadratic;
  // Degrees
  float cut_off;
  float outer_cut_off;

  Light(glm::vec3 position) : position(position),
                              ambient(glm::vec3(0.1)),
                              diffuse(glm::vec3(1.f)),
                              specular(glm::vec3(1.f)),
                              direction(glm::vec3(0.f)),
                              constant(1.f),
                              linear(0.09f),
                              quadratic(0.032f),
                              cut_off(30.f),
                              outer_cut_off(50.f),
                              type(1)
  {
  }

  void send_to_shader(Shaders *program);
};

#endif // !1 LIGHT_H
