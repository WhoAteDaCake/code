#include "Camera.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <limits>

Camera::Camera() : sphere_point(glm::vec3(0.f, 0.f, 0.f)),
                   is_point_valid(false),
                   position(glm::vec3(0.f, 0.f, 6.f)),
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

/**
 * Updates camera sphere and checks if coordinates were in bounds
 */
void Camera::mouse_to_sphere(int x, int y, int width, int height)
{
  // Need to make sure that coordinates are within bounds of screen
  if (x >= 0 && y >= 0 && x <= width && y <= height)
  {
    float sphere_x = static_cast<float>(x - width / 2) / static_cast<float>(width);
    float sphere_y = static_cast<float>(y - height / 2) / static_cast<float>(height);

    float sin_x = sin(M_PI * sphere_x / 2);
    float sin_y = sin(M_PI * sphere_y / 2);
    float product = sin_x * sin_x + sin_y + sin_y;

    this->sphere_point.x = sin_x;
    this->sphere_point.y = sin_y;
    this->sphere_point.z = product < 1.f ? sqrt(1.f - product) : 0.f;
    this->is_point_valid = true;
  }
  this->is_point_valid = false;
}

void Camera::rotate_itself(glm::vec3 axis, float angle)
{
}

void Camera::rotate(int x, int y, int width, int height)
{
  // if (this->is_point_valid)
  // {
  //   glm::vec3 prev_sphere_point = this->sphere_point;
  //   bool is_old_point_ok = this->is_point_valid;
  //   mouse_to_sphere(x, y, width, height);
  //   if (this->is_point_valid)
  //   {
  //     glm::vec3 axis = glm::cross(prev_sphere_point, this->sphere_point);
  //     float cosAngle = glm::dot(prev_sphere_point, this->sphere_point);
  //     float epsilon = std::numeric_limits<float>::epsilon();
  //     if (fabs(cosAngle) < 1.f && axis.length() > epsilon)
  //     {
  //       axis = glm::normalize(axis);
  //       float angle = 2.f * acos(cosAngle);
  //       rotate_itself(axis, -angle);
  //     }
  //   }
  // }
}