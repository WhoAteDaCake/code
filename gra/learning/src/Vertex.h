#pragma once

#ifndef VERTEX_H
#define VERTEX_H

struct Vertex
{
  glm::vec3 position;
  glm::vec3 color;
  glm::vec2 texcoord;
  glm::vec3 normal;
};

#endif