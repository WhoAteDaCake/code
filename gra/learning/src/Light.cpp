#include "Light.h"

void Light::send_to_shader(Shaders *program)
{
  program->use3fv("light.ambient", this->ambient);
  program->use3fv("light.diffuse", this->diffuse);
  program->use3fv("light.specular", this->specular);
  program->use3fv("light.position", this->position);
  program->use1f("light.constant", this->constant);
  program->use1f("light.linear", this->constant);
  program->use1f("light.quadratic", this->constant);
}