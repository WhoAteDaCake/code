#pragma once

#ifndef OBJECT_H
#define OBJECT_H

#include "Mesh.h"
#include "Texture.h"
#include "Material.h"
#include "Log.h"

class Object
{
private:
  std::string name;
  Texture diffuse;
  Texture specular;
  Material material;
  Mesh mesh;

public:
  Object(
      std::string name,
      Texture diffuse,
      Texture specular,
      Material material,
      Mesh mesh) : name(name),
                   diffuse(diffuse),
                   specular(specular),
                   material(material),
                   mesh(mesh)
  {
  }
  ~Object() {}

  void draw(Shaders *program);
  void clear();
  void initialize();
};

#endif // OBJECT_H