#pragma once

#ifndef OBJECT_H
#define OBJECT_H

#include "Mesh.h"
#include "Texture.h"
#include "Material.h"

class Object
{
private:
  Texture diffuse;
  Texture specular;
  Material material;
  Mesh mesh;

public:
  Object();
  ~Object();

  void draw();
  void initialize();
};

#endif // OBJECT_H