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

  std::vector<Vertex> vertices;
  std::vector<GLuint> indices;

  GLuint VAO;
  GLuint VBO;
  GLuint EBO;
  // Dynamic or static
  GLenum draw_type;

  // Transofrmations
  glm::mat4 model_matrix;
  glm::vec3 position;
  glm::vec3 rotation;
  glm::vec3 scale;

  // Setup
  void bind_buffers();
  void calculate_matrix();
  void init_transforms();

  // Actions
  void update_uniforms(Shaders *program);

  std::string error_msg(std::string message)
  {
    return this->name + ": " + message;
  }

public:
  Mesh(std::string name) : vertices(std::vector<Vertex>()),
                           indices(std::vector<GLuint>()),
                           draw_type(GL_DYNAMIC_DRAW),
                           name(name){

                           };
  ~Mesh()
  {
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);
    glDeleteBuffers(1, &this->EBO);
  }

  inline void set_vertices(std::vector<Vertex> &vertices)
  {
    this->vertices = vertices;
  }

  inline void set_inidices(std::vector<GLuint> &indices)
  {
    this->indices = indices;
  }

  inline void set_draw_type(GLenum draw_type)
  {
    if (draw_type != GL_STATIC_DRAW && draw_type != GL_DYNAMIC_DRAW)
    {
      throw error_msg("Incorrect draw type received");
    }
    this->draw_type = draw_type;
  }

  // Setup
  void initialize();

  // Actions
  void render(Shaders *program);
  void update();
};

#endif