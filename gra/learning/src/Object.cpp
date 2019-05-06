#include "Object.h"

void Object::initialize()
{
  this->diffuse.load();
  this->specular.load();
  this->mesh.initialize();
  this->material.set_textures(this->diffuse.get_unit(), this->specular.get_unit());
#ifdef GRA_DEBUG
  Log::check_error("Object:" + this->name + ":initialize");
#endif
}

void Object::draw(Shaders *program)
{
  this->material.send_to_shader(program);

  program->use();
  this->diffuse.bind();
  this->specular.bind();

  this->mesh.draw(program);
#ifdef GRA_DEBUG
  Log::check_error("Object:" + this->name + ":draw");
#endif
}

void Object::clear()
{
  this->diffuse.unbind();
  this->specular.unbind();
#ifdef GRA_DEBUG
  Log::check_error("Object:" + this->name + ":clear")
#endif
}