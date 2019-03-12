#pragma once

#include "DisplayableObject.h"
#include "Animation.h"
#include "Input.h"

class Light : public DisplayableObject, public Animation
{
public:
  Light(GLenum lightSrc, float offset, float lamp_z, int indice);
  ~Light();
  void Display();
  void Update(const double &deltaTime);

private:
  GLfloat *_ambient, *_diffuse, *_specular, *_position;
  GLenum _lightSrc;
};