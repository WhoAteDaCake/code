#include "Light.h"

GLfloat *v4()
{
  GLfloat *v = (GLfloat *)malloc(sizeof(GLfloat) * 4);
  v[0] = 0.f;
  v[1] = 0.f;
  v[2] = 0.f;
  v[3] = 1.f;
  return v;
}

Light::Light(GLenum lightSrc, float offset, float lamp_z, int i)
{
  _ambient = v4();
  _diffuse = v4();
  _specular = v4();
  _position = v4();

  //Set lamp position
  _position[0] = offset;
  _position[2] = lamp_z;
  // Will make either Red, green or blue
  _ambient[i] = 0.5f;
  _diffuse[i] = 0.9f;
  // Set source
  _lightSrc = lightSrc;
}

Light::~Light()
{
  free(_ambient);
  free(_diffuse);
  free(_specular);
  free(_position);
}

void Light::Display()
{
  glPushMatrix();
  glPushAttrib(GL_ALL_ATTRIB_BITS);

  // Make sure the cube is not affected
  glDisable(GL_LIGHTING);
  glColor4fv(_diffuse);

  // Draw and move
  // WARNING: extremely fragile, even changing lamp size can break the whole display
  float lamp_size = 20.f;
  glTranslatef(_position[0], _position[1], _position[2]);
  glutSolidSphere(lamp_size, lamp_size, lamp_size);

  glEnable(GL_LIGHTING);

  glPopAttrib();
  glPopMatrix();
}

void Light::Update(const double &deltaTime)
{
  glEnable(GL_LIGHTING);
  // Colour
  glLightfv(_lightSrc, GL_AMBIENT, _ambient);
  glLightfv(_lightSrc, GL_DIFFUSE, _diffuse);
  glLightfv(_lightSrc, GL_SPECULAR, _specular);

  // Position
  glLightfv(_lightSrc, GL_POSITION, _position);

  glLightf(_lightSrc, GL_CONSTANT_ATTENUATION, 0.5);
  glLightf(_lightSrc, GL_LINEAR_ATTENUATION, 0.025f);
  glLightf(_lightSrc, GL_QUADRATIC_ATTENUATION, 0.00005f);

  glEnable(_lightSrc);
}
