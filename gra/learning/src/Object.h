#pragma once

#ifndef OBJECT_H
#define OBJECT_H

#include "Mesh.h"
#include "TextureManager.h"
#include "Material.h"
#include "Log.h"
#include <memory>
// TMP
#include "Shapes.h"

class Object
{
private:
  typedef std::shared_ptr<Texture> SharedTexture;

  std::string name;
  SharedTexture diffuse;
  SharedTexture specular;
  Material *material;
  Mesh *mesh;

public:
  static Object *from_file(std::string file_name, std::unique_ptr<TextureManager> &manager);

  Object(
      std::string name,
      SharedTexture diffuse,
      SharedTexture specular,
      Material *material,
      Mesh *mesh) : name(name),
                    diffuse(diffuse),
                    specular(specular),
                    material(material),
                    mesh(mesh)
  {
  }
  ~Object()
  {
    delete this->mesh;
    delete this->material;
#ifdef GRA_DEBUG
    Log::log(name + ":" + "destructor");
#endif // DEBUG
  }

  bool has_textures();
  void draw(Shaders *program);
  void clear();
  void initialize();
};

#endif // OBJECT_H