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
      throw "Incorrect draw type received";
    }
    this->draw_type = draw_type;
  }

  void bind_buffers();
  void init_matrix();

public:
};

#endif