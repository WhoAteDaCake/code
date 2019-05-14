#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include "gl.h"
#include "Log.h"

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
  // X rotation
  float yaw;
  // Z rotation
  float pitch;
  // For rotation
  float sensitivity;

  Camera();
  ~Camera();

  void update_view();
  void update_projection(int width, int height);
  void update_front();
  void update_position(glm::vec3 position);
  std::string to_string();

  void add_to_fov(float offset, int width, int height)
  {
    if (fov >= 1.0f && fov <= 90.0f)
      fov += offset;
    if (fov <= 1.0f)
      fov = 1.0f;
    if (fov >= 90.f)
      fov = 90.f;
    update_projection(width, height);
  }

  inline void add_to_yaw(float value)
  {
    this->yaw += value;
  }

  inline void add_to_pitch(float value)
  {
    float new_pitch = this->pitch + value;
    if (new_pitch > 88.f)
    {
      new_pitch = 88.f;
    }
    if (pitch < -88.f)
    {
      new_pitch = -88.f;
    }
    this->pitch = new_pitch;
  }

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
