#include "ObjectLight.h"

void ObjectLight::initialize()
{
  this->object->initialize();
}

void ObjectLight::draw(Shaders *program)
{
  this->light->send_to_shader(program);
  this->object->draw(program);
}

void ObjectLight::update(int delta)
{
  this->object->update(delta);
}

void ObjectLight::clear()
{
  this->object->clear();
}