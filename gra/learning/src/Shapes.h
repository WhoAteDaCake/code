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

  Square(float size) : Mesh("Triangle_" + ID)
  {
    Square::ID += 1;
    this->size = size;
  }

  void initialize()
  {
    float size = this->size;
    Vertex v_arr[] = {
        glm::vec3(-size, size, 0.f),
        glm::vec3(1.f, 0.f, 0.f),
        glm::vec2(0.f, 1.f),
        glm::vec3(0.f, 0.f, 1.f), // Part2
        glm::vec3(-size, -size, 0.f),
        glm::vec3(0.f, 1.f, 0.f),
        glm::vec2(0.f, 0.f),
        glm::vec3(0.f, 0.f, 1.f), // Part3
        glm::vec3(size, -size, 0.f),
        glm::vec3(0.f, 0.f, 1.f),
        glm::vec2(1.f, 0.f),
        glm::vec3(0.f, 0.f, 1.f), // Part4
        glm::vec3(size, size, 0.f),
        glm::vec3(0.f, 0.f, 1.f),
        glm::vec2(1.f, 1.f),
        glm::vec3(0.f, 0.f, 1.f),
    };

    GLint i_arr[] = {
        0, 1, 2, // Triangle 1
        0, 2, 3};

    this->vertices = std::vector<Vertex>(v_arr, v_arr + sizeof(v_arr) / sizeof(v_arr[0]));
    this->indices = std::vector<GLuint>(i_arr, i_arr + sizeof(i_arr) / sizeof(i_arr[0]));
    Mesh::initialize();
  }
};

unsigned int Square::ID = 0;

#endif
