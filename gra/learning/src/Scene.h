#pragma once

#ifndef SCENE_H
#define SCENE_H

#include "Camera.h"
#include "Shaders.h"
#include "Object.h"
#include "Shapes.h"

class Scene
{
private:
  Camera *camera;
  Shaders shader;
  std::vector<Object> objects;

  // TEMPORARY
  glm::vec3 light_pos;
  // Shape square;
  // Texture texture;

public:
  Scene();
  ~Scene();

  void set_camera(Camera *camera);
  void draw();
  void initialize();
};

#endif