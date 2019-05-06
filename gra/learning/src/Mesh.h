#pragma once

#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <vector>

#include "gl.h"
#include "Vertex.h"
#include "Shaders.h"
#include "Texture.h"
#include "Material.h"

class Mesh
{
private:
  std::string name;

  GLuint VAO;
  GLuint VBO;
  GLuint EBO;
  // Dynamic or static
  GLenum draw_type;

  // Transofrmations
  glm::mat4 model_matrix;

  // Setup
  void bind_buffers();
  void calculate_matrix();

  // Actions
  void update_uniforms(Shaders *program);

  std::string error_msg(std::string message)
  {
    return this->name + ": " + message;
  }

public:
  std::vector<Vertex> vertices;
  std::vector<GLuint> indices;

  glm::vec3 position;
  glm::vec3 rotation;
  glm::vec3 scale;

  Mesh(std::string name) : vertices(std::vector<Vertex>()),
                           indices(std::vector<GLuint>()),
                           draw_type(GL_DYNAMIC_DRAW),
                           position(glm::vec3(0.f)),
                           rotation(glm::vec3(0.f)),
                           scale(glm::vec3(1.f)),
                           name(name){

                           };
  ~Mesh()
  {
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);
    glDeleteBuffers(1, &this->EBO);
  }

  // Setup
  virtual void initialize();

  // Actions
  void render(Shaders *program);
  void update();

  inline void set_draw_type(GLenum draw_type)
  {
    if (draw_type != GL_STATIC_DRAW && draw_type != GL_DYNAMIC_DRAW)
    {
      throw error_msg("Incorrect draw type received");
    }
    this->draw_type = draw_type;
  }
};

#endif