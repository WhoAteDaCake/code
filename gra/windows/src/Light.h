#pragma once

#ifndef LIGHT_H
#define LIGHT_H

#include "Shaders.h"

// https://learnopengl.com/Lighting/Light-casters
class Light
{
public:
  /**
   * Each light will get a unique id
   * WIll dictate, which light array index item
   * is sent to the shaders
   * Used to debug and track the number of lights
   */
  static unsigned int ID;
  static unsigned int get_id();

  /**
   * 0 - Directional 
   * 1 - Point light
   * 2 - Spotlight
   */
  int type;
  unsigned int id;
  // Whether blinn specular shading is used
  bool blinn;

  /**
   * Full property identifier name used
   * to get uniform ids and send light properties
   */
  std::string full;
  /**
   * Only need to calculate once
   * and then reused when enabling light in shaders
   * light[+ ID +].
   */
  std::string prefix;

  /**
   * 3D properties used to calculate
   * effects of the light in the environment
   */
  glm::vec3 position;
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  glm::vec3 direction;
  // Strength of the light
  float constant;
  float linear;
  float quadratic;
  // Spotlight cutoff angles
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
  /**
   * Send all light properties to shader
   */
  void send_to_shader(Shaders *program);
  /**
   * Will copy all light properties from another light
   */
  void copy_properties(Light *light, bool copy_position);
  /**
   * Used to create const char* that's used
   * in getUniformLocation
   */
  const char *field(std::string name);
};

#endif // !1 LIGHT_H
