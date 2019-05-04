
#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H

#include "gl.h"
#include "SOIL.h"
#include <string>
#include "Log.h"

class Texture
{
private:
  int width;
  int height;
  GLuint id;
  GLenum type;
  GLuint unit;

  GLuint load_texture(std::string file, GLenum type);

public:
  Texture(GLenum type, GLint unit)
  {
    this->width = 0;
    this->height = 0;
    this->id = 0;
    this->type = type;
    this->unit = unit;
  }

  ~Texture()
  {
    glDeleteTextures(1, &this->id);
  }

  void load(std::string name)
  {
    this->id = load_texture(name, this->type);
  }

  void bind()
  {
    glActiveTexture(GL_TEXTURE0 + this->unit);
    glBindTexture(this->type, this->id);
    Log::check_error("Failed to bind texture: " + this->id);
  }

  void unbind()
  {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(this->type, GL_TEXTURE0);
    Log::check_error("Failed to unbind texture: " + this->id);
  }

  inline GLuint get_id() const
  {
    return this->id;
  }

  inline GLuint get_unit() const
  {
    return this->unit;
  }
};

#endif