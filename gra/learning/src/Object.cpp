#include "Object.h"

bool Object::has_textures()
{
  return this->diffuse != nullptr && this->specular != nullptr;
}

void Object::initialize()
{
  this->mesh->initialize();

  if (has_textures())
  {
    this->material->set_textures(this->diffuse->get_unit(), this->specular->get_unit());
  }

#ifdef GRA_DEBUG
  Log::check_error("Object:" + this->name + ":initialize");
#endif
}

void Object::draw(Shaders *program)
{

  this->material->send_to_shader(program);

  program->use();
  if (has_textures())
  {
    this->diffuse->bind();
    this->specular->bind();
  }
  this->mesh->draw(program);
}

void Object::clear()
{
  if (has_textures())
  {
    this->diffuse->unbind();
    this->specular->unbind();
  }
}