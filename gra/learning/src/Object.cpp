#include "Object.h"

void Object::initialize()
{
  this->diffuse.load();
  this->specular.load();
  this->mesh.initialize();
  this->material.set_textures(this->diffuse.get_id(), this->specular.get_id());
  Log::check_error("Object:" + this->name + ":initialize");
}

void Object::draw(Shaders *program)
{
  this->material.send_to_shader(program);
  this->diffuse.bind();
  this->specular.bind();

  this->mesh.draw(program);
}

void Object::clear()
{
  this->diffuse.unbind();
  this->specular.unbind();
}