#include "Camera.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <limits>

Camera::Camera() : position(glm::vec3(0.f, 0.f, 6.f)),
                   world_up(glm::vec3(0.f, 1.f, 0.f)),
                   front(glm::vec3(0.f, 0.f, -1.f)),
                   view_matrix(glm::mat4(1.f)),
                   fov(90.f),
                   near_plane(0.1f),
                   far_plane(150.f),
                   projection_matrix(glm::mat4(1.f)),
                   yaw(-90.f),
                   pitch(0.f),
                   sensitivity(0.6f)
{
  update_front();
}

Camera::~Camera() {}

void Camera::update_view()
{
  this->view_matrix = glm::lookAt(this->position, this->position + this->front, this->world_up);
}

void Camera::update_projection(int width, int height)
{
  this->projection_matrix = glm::perspective(
      glm::radians(this->fov),
      static_cast<float>(width) / static_cast<float>(height),
      this->near_plane,
      this->far_plane);
}

void Camera::update_front()
{
  glm::vec3 new_front;
  new_front.x = cos(glm::radians(this->pitch)) * cos(glm::radians(this->yaw));
  new_front.y = sin(glm::radians(this->pitch));
  new_front.z = cos(glm::radians(this->pitch)) * sin(glm::radians(this->yaw));
  this->front = glm::normalize(new_front);
  update_view();
}

void Camera::update_position(glm::vec3 position)
{
  this->position = position;
  update_view();
}

std::string Camera::to_string()
{
  std::string result = "";
  result += "Position: " + glm::to_string(this->position) + "\n";
  result += "Front: " + glm::to_string(this->front);
  return result;
}