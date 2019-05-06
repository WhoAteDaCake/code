#include "Camera.h"

Camera::Camera() : position(glm::vec3(0.f, 0.f, 6.f)),
                   world_up(glm::vec3(0.f, 1.f, 0.f)),
                   front(glm::vec3(0.f, 0.f, -1.f)),
                   view_matrix(glm::mat4(1.f)),
                   fov(90.f),
                   near_plane(0.1f),
                   far_plane(100.f),
                   projection_matrix(glm::mat4(1.f))
{
  update_view();
}

Camera::~Camera() {}

void Camera::update_view()
{
  this->view_matrix = glm::lookAt(this->position, this->position + this->front, this->world_up);
#ifdef GRA_DEBUG
  Log::log("Camera: updating view matrix");
#endif // DEBUG
}

void Camera::update_projection(int width, int height)
{
  this->projection_matrix = glm::perspective(
      glm::radians(this->fov),
      static_cast<float>(width) / static_cast<float>(height),
      this->near_plane,
      this->far_plane);

#ifdef GRA_DEBUG
  Log::log("Camera: updating view projection");
#endif // DEBUG
}