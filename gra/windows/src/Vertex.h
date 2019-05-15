#pragma once

#ifndef VERTEX_H
#define VERTEX_H

/**
 * Structure used to draw items on the screen
 */
struct Vertex
{
  glm::vec3 position;
  glm::vec3 color;
  glm::vec2 texcoord;
  glm::vec3 normal;

  Vertex() : position(glm::vec3(0.f, 0.f, 0.f)), color(0.f, 0.f, 0.f), texcoord(glm::vec2(0.f, 0.f)), normal(glm::vec3(0.f, 0.f, 0.f)) {}
  Vertex(glm::vec3 position,
         glm::vec3 color,
         glm::vec2 texcoord,
         glm::vec3 normal) : position(position),
                             color(color),
                             texcoord(texcoord),
                             normal(normal)
  {
  }
};

#endif