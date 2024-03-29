#pragma once

#ifndef SHAPES_H
#define SHAPES_H

#define M_PI 3.14159265358979323846  /* pi */
#include "Mesh.h"

/**
 * Defines creation of a square mesh
 */
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


/**
 * Defines creation of a skybox
 */
class SkyboxMesh : public Mesh
{
protected:
  void bind_buffers()
  {
    float skyboxVertices[] = {
        -10.0f, 10.0f, -10.0f,
        -10.0f, -10.0f, -10.0f,
        10.0f, -10.0f, -10.0f,
        10.0f, -10.0f, -10.0f,
        10.0f, 10.0f, -10.0f,
        -10.0f, 10.0f, -10.0f,

        -10.0f, -10.0f, 10.0f,
        -10.0f, -10.0f, -10.0f,
        -10.0f, 10.0f, -10.0f,
        -10.0f, 10.0f, -10.0f,
        -10.0f, 10.0f, 10.0f,
        -10.0f, -10.0f, 10.0f,

        10.0f, -10.0f, -10.0f,
        10.0f, -10.0f, 10.0f,
        10.0f, 10.0f, 10.0f,
        10.0f, 10.0f, 10.0f,
        10.0f, 10.0f, -10.0f,
        10.0f, -10.0f, -10.0f,

        -10.0f, -10.0f, 10.0f,
        -10.0f, 10.0f, 10.0f,
        10.0f, 10.0f, 10.0f,
        10.0f, 10.0f, 10.0f,
        10.0f, -10.0f, 10.0f,
        -10.0f, -10.0f, 10.0f,

        -10.0f, 10.0f, -10.0f,
        10.0f, 10.0f, -10.0f,
        10.0f, 10.0f, 10.0f,
        10.0f, 10.0f, 10.0f,
        -10.0f, 10.0f, 10.0f,
        -10.0f, 10.0f, -10.0f,

        -10.0f, -10.0f, -10.0f,
        -10.0f, -10.0f, 10.0f,
        10.0f, -10.0f, -10.0f,
        10.0f, -10.0f, -10.0f,
        -10.0f, -10.0f, 10.0f,
        10.0f, -10.0f, 10.0f};

    glGenBuffers(1, &this->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, 3 * 36 * sizeof(float), &skyboxVertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);
    glBindVertexArray(0);
  }

public:
  SkyboxMesh(std::string name) : Mesh(name)
  {
  }

  void draw(Shaders *program)
  {
    glDisable(GL_DEPTH_TEST);

    glDepthFunc(GL_LEQUAL); // change depth function so depth test passes when values are equal to depth buffer's content
    program->use();

    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
  }

  void initialize(glm::mat4 inital_matrix)
  {
    // Don't want to throw the error
    bind_buffers();
  }
};



/**
 * Defines creation of a HalfSphere
 */
class HalfSphere : public Mesh
{
private:
  float size;
  glm::vec3 color;
  int curve;

  void add_vertex(glm::vec3 position)
  {
    this->vertices.push_back(Vertex(position, this->color, glm::vec2(0.f), glm::vec3(0.f)));
  }

  /**
   * Convert triangle point to a sphere point
   */
  glm::vec3 curved_middle_point(glm::vec3 p1, glm::vec3 p2)
  {
    glm::vec3 mp = (p1 + p2) / 2.f;
    // Distance to center point (0, 0, 0)
    float dtc = sqrt(glm::compAdd(mp * mp));
    // The ratio of sphere radius divided by distance to the center
    // Helps us to calculate how much shift is needed for the coordinate to be
    // part of the sphere surface
    float md = this->size / dtc;
    mp *= md;
    return mp;
  }

  /**
   * Subdivide the triangle to create more detailed mesh
   */
  void subdivide(int level, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
  {
    if (level == 0)
    {
      add_vertex(p1);
      add_vertex(p2);
      add_vertex(p3);
      return;
    }
    glm::vec3 lp = curved_middle_point(p1, p2);
    glm::vec3 bp = curved_middle_point(p2, p3);
    glm::vec3 rp = curved_middle_point(p1, p3);

    int new_level = level - 1;
    // Left
    subdivide(new_level, lp, p2, bp);
    // Middle
    subdivide(new_level, lp, bp, rp);
    // Right
    subdivide(new_level, rp, bp, p3);
    // Top
    subdivide(new_level, p1, lp, rp);
  }

public:
  HalfSphere(
      std::string name,
      glm::vec3 color,
      int curve) : Mesh(name),
                   color(color),
                   curve(curve),
                   size(0.5f){};

  void initialize(glm::mat4 initial_matrix)
  {
    float radius = this->size;
    float level = curve;
    glm::vec3 p1;
    glm::vec3 p2;
    glm::vec3 p3;

    // Front down Face
    p1 = glm::vec3(0.f, 0.f, radius);
    p2 = glm::vec3(0.f, -radius, 0.f);
    p3 = glm::vec3(radius, 0.f, 0.f);
    subdivide(level, p1, p2, p3);

    // Left down
    p1 = glm::vec3(0.f, 0.f, radius);
    p2 = glm::vec3(-radius, 0.f, 0.f);
    p3 = glm::vec3(0.f, -radius, 0.f);
    subdivide(level, p1, p2, p3);

    // Right down
    p1 = glm::vec3(radius, 0.f, 0.f);
    p3 = glm::vec3(0.f, 0.f, -radius);
    p2 = glm::vec3(0.f, -radius, 0.f);
    subdivide(level, p1, p2, p3);

    // Back down
    p1 = glm::vec3(0.f, 0.f, -radius);
    p3 = glm::vec3(-radius, 0.f, 0.f);
    p2 = glm::vec3(0.f, -radius, 0.f);
    subdivide(level, p1, p2, p3);

    Mesh::initialize(initial_matrix);
  }
};

/**
 * Creates a cylinder mesh
 */
class Cylinder : public Mesh
{
private:
  glm::vec3 color;

public:
  Cylinder(std::string name, glm::vec3 color) : Mesh(name), color(color) {}

  void add_vertex(float x, float y, float z)
  {
    this->vertices.push_back(Vertex(glm::vec3(x, y, z), this->color, glm::vec2(0.f), glm::vec3(0.f)));
  }

  void initialize(glm::mat4 initial_matrix)
  {
    // Size will be 2x radius
    float radius = 0.5f;
    float height = 1.f;
    float slices = 360.f;
    float DEG2RAD = M_PI / 180;

    float prev = 0.f;
    for (int i = 1.f; i <= slices; i++)
    {
      float deg_prev = prev * DEG2RAD;
      float deg_now = i * DEG2RAD;

      float px = cos(deg_prev) * radius;
      float pz = sin(deg_prev) * radius;

      float nx = cos(deg_now) * radius;
      float nz = sin(deg_now) * radius;

      // Bottom cicle
      add_vertex(0.f, 0.f, 0.f);
      add_vertex(px, 0.f, pz);
      add_vertex(nx, 0.f, nz);

      // Top circle
      add_vertex(0.f, height, 0.f);
      add_vertex(nx, height, nz);
      add_vertex(px, height, pz);

      // Wall 1
      add_vertex(px, 0.f, pz);
      add_vertex(px, height, pz);
      add_vertex(nx, height, nz);
      // Wall 2
      add_vertex(px, 0.f, pz);
      add_vertex(nx, height, nz);
      add_vertex(nx, 0.f, nz);

      prev = i;
    }
    Mesh::initialize(initial_matrix);
  }
};

#endif
