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
protected:
  std::string name;

private:
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

  std::string message(std::string message)
  {
    std::string ending = ":" + message;
    return this->name + ending;
  }

public:
  std::vector<Vertex> vertices;
  std::vector<GLuint> indices;
  std::string texture_name;

  glm::vec3 position;
  glm::vec3 rotation;
  glm::vec3 scale;

  Mesh(std::string name) : vertices(std::vector<Vertex>()),
                           indices(std::vector<GLuint>()),
                           draw_type(GL_STATIC_DRAW),
                           position(glm::vec3(0.f)),
                           rotation(glm::vec3(0.f)),
                           scale(glm::vec3(1.f)),
                           name(name),
                           texture_name(""){

                           };
  ~Mesh()
  {
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);
    if (has_indices())
    {
      glDeleteBuffers(1, &this->EBO);
    }

#ifdef GRA_DEBU
    Log::log(message("destructor call"));
#endif // DEBUG
  }

  // Setup
  virtual void initialize();

  // Actions
  void draw(Shaders *program);
  void update();

  inline bool has_indices()
  {
    return this->indices.size() != 0;
  }

  inline void set_draw_type(GLenum draw_type)
  {
    if (draw_type != GL_STATIC_DRAW && draw_type != GL_DYNAMIC_DRAW)
    {
      throw message("Incorrect draw type received");
    }
    this->draw_type = draw_type;
  }
};

#endif