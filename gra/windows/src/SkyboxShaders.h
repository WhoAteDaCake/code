#pragma once

#ifndef SKYBOX_SHADERS_H
#define SKYBOX_SHADERS_H

#include "Shaders.h"

class SkyboxShaders : public Shaders
{
protected:
  void bind_attrib(GLuint program)
  {
    glBindAttribLocation(program, 0, "aPos");
  }

public:
  SkyboxShaders() : Shaders() {}
};

#endif // !SHADERS_X