#pragma once

#ifndef SHADERS_H
#define SHADERS_H

#include <fstream>
#include "gl.h"
#include "Log.h"

class Shaders
{
private:
  GLuint id;

  std::string shader_src(std::string name);
  GLuint load_shader(std::string name, GLenum type);
  GLuint link_shaders(std::string vertex, std::string fragment, std::string geometry);

public:
  Shaders()
  {
    this->id = 0; //link_shaders(vertex, fragment, geometry);
  }

  ~Shaders()
  {
    glDeleteProgram(this->id);
  }

  void set_shaders(std::string vertex, std::string fragment, std::string geometry = "")
  {
    this->id = link_shaders(vertex, fragment, geometry);
  }

  void use()
  {
    glUseProgram(this->id);
    Log::check_error("Using shader");
  }

  void stop_use()
  {
    glUseProgram(0);
    Log::check_error("Stopping shader use");
  }

  void use1i(const GLchar *name, int i)
  {
    use();
    glUniform1i(glGetUniformLocation(this->id, name), i);
    Log::check_error(name);
    stop_use();
  }

  void useM4fv(const GLchar *name, glm::mat4 mat)
  {
    use();
    glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, GL_FALSE, glm::value_ptr(mat));
    Log::check_error(name);
    stop_use();
  }

  void use3fv(const GLchar *name, glm::vec3 vect)
  {
    use();
    glUniform3fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(vect));
    Log::check_error(name);
    stop_use();
  }
};

#endif