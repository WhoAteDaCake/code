#pragma once

#ifndef SHAPES_H
#define SHAPES_H

#include "Mesh.h"

class Square : public Mesh
{
private:
  float size;

public:
  static unsigned int ID;

  Square(std::string name, float size) : Mesh(name)
  {
    this->size = size;
  }

  virtual void initialize(glm::mat4 inital_matrix)
  {
    float size = this->size;
    Vertex v_arr[] = {
        Vertex(
            glm::vec3(-size, size, 0.f),
            glm::vec3(1.f, 0.f, 0.f),
            glm::vec2(0.f, 1.f),
            glm::vec3(0.f, 0.f, 1.f)), // Part2
        Vertex(
            glm::vec3(-size, -size, 0.f),
            glm::vec3(0.f, 1.f, 0.f),
            glm::vec2(0.f, 0.f),
            glm::vec3(0.f, 0.f, 1.f)), // Part3
        Vertex(
            glm::vec3(size, -size, 0.f),
            glm::vec3(0.f, 0.f, 1.f),
            glm::vec2(1.f, 0.f),
            glm::vec3(0.f, 0.f, 1.f)), // Part4
        Vertex(
            glm::vec3(size, size, 0.f),
            glm::vec3(0.f, 0.f, 1.f),
            glm::vec2(1.f, 1.f),
            glm::vec3(0.f, 0.f, 1.f)),
    };

    GLint i_arr[] = {
        0, 1, 2, // Triangle 1
        0, 2, 3};

    unsigned nrOfVertices = sizeof(v_arr) / sizeof(Vertex);
    unsigned nrOfIndices = sizeof(i_arr) / sizeof(GLuint);

    std::vector<Vertex> v;
    std::vector<GLuint> ind;

    for (int i = 0; i < nrOfVertices; i += 1)
    {
      v.push_back(v_arr[i]);
    }
    for (int i = 0; i < nrOfIndices; i += 1)
    {
      ind.push_back(i_arr[i]);
    }
    this->vertices = v;
    this->indices = ind;
    Mesh::initialize(inital_matrix);
  }
};

#endif
