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

public:
  Material() {}
  ~Material() {}

  void set(glm::vec3 ambient,
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

  void send_to_shader(Shaders program)
  {
    program.use3fv("material.ambient", this->ambient);
    program.use3fv("material.diffuse", this->diffuse);
    program.use3fv("material.specular", this->specular);
    program.use1i("material.diffuse_tex", this->diffuse_tex);
    program.use1i("material.specular_tex", this->specular_tex);
  }
};

#endif