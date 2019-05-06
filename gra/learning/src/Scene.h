#pragma once

#ifndef SCENE_H
#define SCENE_H

#include "Camera.h"
#include "Object.h"
#include "Shapes.h"
#include "TextureManager.h"

class Scene
{
private:
  Camera *camera;
  Shaders shader;
  std::vector<std::unique_ptr<Object>> objects;
  std::unique_ptr<TextureManager> texture_manager;

  // TEMPORARY
  glm::vec3 light_pos;
  // Shape square;
  // Texture texture;

public:
  Scene();
  ~Scene();

  // Can't allow to be coppied becayse of unique_pts
  Scene &operator=(const Scene &) = delete; // non copyable
  Scene(const Scene &) = delete;            // non construction-copyable

  void set_camera(Camera *camera);
  void draw();
  void initialize();
  void clear();
};

#endif