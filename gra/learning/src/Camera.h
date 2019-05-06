#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include "gl.h"

class Camera
{
private:
  glm::mat4 projection_matrix;
  glm::mat4 view_matrix;

public:
  glm::vec3 position;
  glm::vec3 world_up;
  glm::vec3 front;
  // Projections
  float fov;
  // Want this behind the camera to avoid clipping
  float near_plane;
  // Draw distance
  float far_plane;
  Camera();
  ~Camera();

  void update_view();
  void update_projection(int width, int height);

  inline glm::mat4 get_projection_matrix()
  {
    return this->projection_matrix;
  }
  inline glm::mat4 get_view_matrix()
  {
    return this->view_matrix;
  }
};

#endif
