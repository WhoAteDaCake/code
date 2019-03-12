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
  GLenum _lightSrc;

  GLfloat *_ambient, *_diffuse, *_specular, *_position;

  GLfloat _c_falloff, _l_falloff, _q_falloff;
};