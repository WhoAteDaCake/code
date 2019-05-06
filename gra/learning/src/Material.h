#pragma once

#ifndef MATERIAL_H
#define MATERIAL_H

#include <iostream>
#include "gl.h"
#include "Shaders.h"
class Material
{
private:
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  GLint diffuse_tex;
  GLint specular_tex;
  bool has_textures;
  bool show_color;

public:
  Material(glm::vec3 ambient,
           glm::vec3 diffuse,
           glm::vec3 specular) : ambient(ambient),
                                 diffuse(diffuse),
                                 specular(specular),
                                 has_textures(false),
                                 show_color(true)
  {
  }
  ~Material()
  {
#ifdef GRA_DEBUG
    Log::log("Material:destructor");
#endif // DEBUG
  }

  void toggle_color(bool show_color)
  {
    this->show_color = show_color;
  }

  void set_specs(glm::vec3 ambient,
                 glm::vec3 diffuse,
                 glm::vec3 specular)
  {
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
  }

  void set_textures(GLint diffuse_tex, GLint specular_tex)
  {
    this->diffuse_tex = diffuse_tex;
    this->specular_tex = specular_tex;
    this->has_textures = true;
  }

  void send_to_shader(Shaders *program)
  {
    program->use3fv("material.ambient", this->ambient);
    program->use3fv("material.diffuse", this->diffuse);
    program->use3fv("material.specular", this->specular);
    if (has_textures)
    {
      program->use1i("mat_diffuse_tex", this->diffuse_tex);
      program->use1i("mat_specular_tex", this->specular_tex);
    }
    program->use1i("mat_has_tex", this->has_textures ? 1 : 0);
    program->use1i("show_color", this->show_color ? 1 : 0);
#ifdef GRA_DEBUG
    Log::check_error("Sending to shader");
#endif // DEBUG
  }
};

#endif