#include "Light.h"

unsigned int Light::ID = 0;

unsigned int Light::get_id()
{
  auto id = Light::ID;
  Light::ID += 1;
  return id;
}

void Light::send_to_shader(Shaders *program)
{
  program->use3fv("light.position", this->position);
  program->use3fv("light.direction", this->direction);
  program->use3fv("light.ambient", this->ambient);
  program->use3fv("light.diffuse", this->diffuse);
  program->use3fv("light.specular", this->specular);
  program->use1f("light.constant", this->constant);
  program->use1f("light.linear", this->linear);
  program->use1f("light.quadratic", this->quadratic);
  program->use1f("light.cut_off", glm::cos(glm::radians(this->cut_off)));
  program->use1f("light.outer_cut_off", glm::cos(glm::radians(this->outer_cut_off)));

  program->use1i("light.type", this->type);
}