#pragma once

#ifndef LIGHT_H
#define LIGHT_H

#include "Shaders.h"

// https://learnopengl.com/Lighting/Light-casters
class Light
{
public:
  static unsigned int ID;
  static unsigned int get_id();

  /**
   * 0 - Directional 
   * 1 - Point light
   * 2 - Spotlight
   */
  int type;
  unsigned int id;
  bool blinn;

  std::string full;
  std::string prefix;

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

  // Can't allow to be coppied becayse of unique_pts
  Light &operator=(const Light &) = delete; // non copyable
  Light(const Light &) = delete;            // non construction-copyable

  Light(glm::vec3 position) : position(position),
                              ambient(glm::vec3(0.1f)),
                              diffuse(glm::vec3(0.f)),
                              specular(glm::vec3(0.4f)),
                              direction(glm::vec3(0.f)),
                              constant(1.f),
                              linear(0.022f),
                              quadratic(0.0019f),
                              cut_off(30.f),
                              outer_cut_off(50.f),
                              type(1),
                              id(Light::get_id()),
                              prefix(""), full(""),
                              blinn(true)
  {
    this->prefix = std::string("lights[") + std::to_string(this->id) + std::string("].");
  }

  void send_to_shader(Shaders *program);
  void copy_properties(Light *light, bool copy_position);
  const char *field(std::string name);
};

#endif // !1 LIGHT_H
