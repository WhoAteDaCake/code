#pragma once

#ifndef MATERIAL_H
#define MATERIAL_H

#include <iostream>
#include "gl.h"
#include "Shaders.h"
class Material
{
private:
  // Unit defined when a texture is registered
  GLint diffuse_tex;
  GLint specular_tex;

  /**
   * Allows to enable and disable
   * specific properties of the material
   */
  bool has_diffuse_tex;
  bool has_specular_tex;
  bool show_color;
  bool ignore_light;

public:
  /**
   * Color properties
   */
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  float shiness;
  /**
   * Used for identification
   */
  std::string diffuse_tex_name;

  Material(glm::vec3 ambient,
           glm::vec3 diffuse,
           glm::vec3 specular,
           bool show_color) : ambient(ambient),
                              diffuse(diffuse),
                              specular(specular),
                              has_diffuse_tex(false),
                              has_specular_tex(false),
                              show_color(show_color),
                              diffuse_tex(-1),
                              specular_tex(-1),
                              diffuse_tex_name(""),
                              shiness(8.f),
                              ignore_light(false)
  {
  }

  Material(glm::vec3 ambient,
           glm::vec3 diffuse,
           glm::vec3 specular) : Material(ambient, diffuse, specular, false)
  {
  }

  Material(const Material &m2) : Material(m2.ambient, m2.diffuse, m2.specular) {}

  ~Material()
  {
#ifdef GRA_DEBUG
    Log::log("Material:destructor");
#endif // DEBUG
  }
  /**
   * Allows to ignore light properties on
   * specific materials
   */
  void toggle_ignore_light(bool toggle)
  {
    this->ignore_light = toggle;
  }
  /**
   * Used to diable vertex defined color
   */
  void toggle_color(bool show_color)
  {
    this->show_color = show_color;
  }

  /**
   * Set color properties
   */
  void set_specs(glm::vec3 ambient,
                 glm::vec3 diffuse,
                 glm::vec3 specular)
  {
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
  }
  /**
   * Assign registered textures
   */
  void set_textures(GLint diffuse_tex, GLint specular_tex)
  {
    this->diffuse_tex = diffuse_tex;
    this->specular_tex = specular_tex;
    this->has_diffuse_tex = diffuse_tex != -1;
    this->has_specular_tex = specular_tex != -1;
  }
  /**
   * Send material to shaders for vertex to be drawn
   */
  void send_to_shader(Shaders *program)
  {
    program->use3fv("material.ambient", this->ambient);
    program->use3fv("material.diffuse", this->diffuse);
    program->use3fv("material.specular", this->specular);
    program->use1f("material.shiness", this->shiness);

    program->use1i("material.has_diffuse", this->has_diffuse_tex ? 1 : 0);
    program->use1i("material.has_specular", this->has_specular_tex ? 1 : 0);
    program->use1i("material.show_color", this->show_color ? 1 : 0);
    program->use1i("material.ignore_light", this->ignore_light ? 1 : 0);
    if (this->has_diffuse_tex)
    {
      program->use1i("mat_diffuse_tex", this->diffuse_tex);
    }
    if (this->has_specular_tex)
    {
      program->use1i("mat_specular_tex", this->specular_tex);
    }
#ifdef GRA_DEBUG
    Log::check_error("Sending to shader");
#endif // DEBUG
  }
};

#endif