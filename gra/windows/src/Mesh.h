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

/**
 * Defines all of the vertices and indices used to 
 * draw items on the screen
 */
class Mesh
{
protected:
  std::string name;

  /**
   * Used for aligning sent vertex data
   */
  void set_pointers();
  /**
   * Binds data to VAO, VBO and EBO
   */
  virtual void bind_buffers();
  // Assigned ids
  GLuint VAO;
  GLuint VBO;
  GLuint EBO;
  // Dynamic or static
  GLenum draw_type;

  // Transofrmations
  glm::mat4 model_matrix;

  /**
   * Applies all 3d transformations to the matrix
   */
  void calculate_matrix(glm::mat4 initial_matrix);

  /**
   * Used for creating log messages to indicate name of the mesh
   */
  std::string message(std::string message)
  {
    std::string ending = ":" + message;
    return this->name + ending;
  }

public:
  /**
   * Data drawn to the screen
   */
  std::vector<Vertex> vertices;
  std::vector<GLuint> indices;
  std::string mat_name;

  /**
   * Transformations in 3d space
   */
  glm::vec3 position;
  glm::vec3 rotation;
  glm::vec3 scale;
  /**
   * Allows to automatically calculate normals
   * Using glm
   */
  bool auto_normals;
  // For when we require to use transforms of a previous mesh
  // In the object
  int dependency_index;

  Mesh(std::string name, int dependency_index) : vertices(std::vector<Vertex>()),
                                                 indices(std::vector<GLuint>()),
                                                 draw_type(GL_STATIC_DRAW),
                                                 position(glm::vec3(0.f)),
                                                 rotation(glm::vec3(0.f)),
                                                 scale(glm::vec3(1.f)),
                                                 name(name),
                                                 mat_name(""),
                                                 dependency_index(dependency_index),
                                                 auto_normals(false){

                                                 };
  Mesh(std::string name) : Mesh(name, -1){};

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

  /**
   * Bind to bufferes and calculate model matrix
   */
  virtual void initialize(glm::mat4 initial_matrix);
  /**
   * Automatic normal calculation
   */
  void calculate_normals();
  /**
   * Draw to the screen
   */
  virtual void draw(Shaders *program);
  /**
   * Update model matrix based on changes to
   * 3d transforms
   */
  void update(glm::mat4 initial_matrix);

  inline std::string get_name()
  {
    return this->name;
  }

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

  inline const glm::mat4 get_model_matrix()
  {
    return this->model_matrix;
  }
};

#endif