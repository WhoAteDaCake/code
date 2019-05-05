#pragma once

#ifndef MATERIAL_H
#define MATERIAL_H

#include <iostream>
#include "gl.h"
#include "Shaders.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

class Material
{
private:
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  GLint diffuse_tex;
  GLint specular_tex;

public:
  Material() {}
  ~Material() {}

  void set_specs(glm::vec3 ambient,
                 glm::vec3 diffuse,
                 glm::vec3 specular,
                 GLint diffuse_tex,
                 GLint specular_tex)
  {
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
    this->diffuse_tex = diffuse_tex;
    this->specular_tex = specular_tex;
  }

  void send_to_shader(Shaders &program)
  {
    program.use3fv("material.ambient", this->ambient);
    program.use3fv("material.diffuse", this->diffuse);
    program.use3fv("material.specular", this->specular);
    program.use1i("mat_diffuse_tex", this->diffuse_tex);
    program.use1i("mat_specular_tex", this->specular_tex);
    Log::check_error("Sending to shader");
  }
};

#endif