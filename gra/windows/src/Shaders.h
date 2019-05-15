#pragma once

#ifndef SHADERS_H
#define SHADERS_H

#include <fstream>
#include "gl.h"
#include "Log.h"

class Shaders
{
protected:
  GLuint id;

  /** Reads shader file into a string */
  std::string shader_src(std::string name);
  /** Creates the shader inside OpenGL context */
  GLuint load_shader(std::string name, GLenum type);
  /** Binds the order of data sent to shader */
  virtual void bind_attrib(GLuint program);
  /** Link all of the different shaders */
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

  /**
   * Allow shader to be used by OpenGL
   */
  void use()
  {
    glUseProgram(this->id);
#ifdef GRA_DEBUG

    Log::check_error("Using shader");
#endif // DEBUG
  }
  /**
   * Remove shader form openGL context
   */
  void stop_use()
  {
    glUseProgram(0);
#ifdef GRA_DEBUG

    Log::check_error("Stopping shader use");
#endif // DEBUG
  }

  /**
   * Send different types of data to the shader
   */
  void use1i(const GLchar *name, int i)
  {
    use();
    glUniform1i(glGetUniformLocation(this->id, name), i);
    Log::check_error(name);
    stop_use();
  }

  void use1f(const GLchar *name, float i)
  {
    use();
    glUniform1f(glGetUniformLocation(this->id, name), i);
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