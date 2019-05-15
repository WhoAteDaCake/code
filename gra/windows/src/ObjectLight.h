#pragma once

#ifndef OJBECT_LIGHT_H
#define OJBECT_LIGHT_H

#include "Light.h"
#include "Object.h"

/**
 * Groups Object and Light class instances together
 */
class ObjectLight
{
public:
  std::unique_ptr<Light> light;
  std::unique_ptr<Object> object;

  // Can't allow to be coppied becayse of unique_pts
  ObjectLight &operator=(const ObjectLight &) = delete; // non copyable
  ObjectLight(const ObjectLight &) = delete;            // non construction-copyable

  ObjectLight(
      std::unique_ptr<Object> object,
      std::unique_ptr<Light> light) : object(std::move(object)),
                                      light(std::move(light))
  {
  }
  /**
   * Initialize object stored inside
   */
  void initialize();
  /**
   * Send light and object to shaders
   */
  void draw(Shaders *program);
  /**
   * Update object and light for animations
   */
  void update(int delta);
  /**
   * Unsassign object and light from gl space
   */
  void clear();
  
  void set_position(glm::vec3 pos)
  {
    this->light->position = pos;
    this->object->update_position(pos);
  }

  void set_scale(glm::vec3 scale)
  {
    this->object->update_scale(scale);
  }

  inline glm::vec3 get_position()
  {
    return this->object->get_position();
  }
};

#endif // !OJBECT_LIGHT_H