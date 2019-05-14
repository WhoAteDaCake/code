#include "Light.h"

unsigned int Light::ID = 0;

unsigned int Light::get_id()
{
  auto id = Light::ID;
  Light::ID += 1;
  return id;
}

const char *Light::field(std::string name)
{
  // Need to maintain in memory, otherwise c_str points to null
  this->full = this->prefix + name;
  return full.c_str();
}
void Light::send_to_shader(Shaders *program)
{
  program->use3fv(field("position"), this->position);
  program->use3fv(field("direction"), this->direction);
  program->use3fv(field("ambient"), this->ambient);
  program->use3fv(field("diffuse"), this->diffuse);
  program->use3fv(field("specular"), this->specular);
  program->use1f(field("constant"), this->constant);
  program->use1f(field("linear"), this->linear);
  program->use1f(field("quadratic"), this->quadratic);
  program->use1f(field("cut_off"), glm::cos(glm::radians(this->cut_off)));
  program->use1f(field("outer_cut_off"), glm::cos(glm::radians(this->outer_cut_off)));

  program->use1i(field("type"), this->type);
  program->use1i(field("blinn"), this->blinn ? 1 : 0);
}

void Light::copy_properties(Light *light, bool copy_position)
{
  // General
  if (copy_position)
  {
    this->position = light->position;
  }
  this->ambient = light->ambient;
  this->diffuse = light->diffuse;
  this->specular = light->specular;
  this->direction = light->direction;
  // Strength
  this->constant = light->constant;
  this->linear = light->linear;
  this->quadratic = light->quadratic;
  // Degrees
  this->cut_off = light->cut_off;
  this->outer_cut_off = light->outer_cut_off;
  // Type
  this->type = light->type;
  this->blinn = light->blinn;
}