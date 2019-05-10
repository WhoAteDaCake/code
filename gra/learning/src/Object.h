#pragma once

#ifndef OBJECT_H
#define OBJECT_H

#include "Mesh.h"
#include "TextureManager.h"
#include "Material.h"
#include "Log.h"
#include <memory>
#include <vector>
// TMP
#include "Shapes.h"
#include "FileLoader.h"

class Object
{
private:
  typedef std::shared_ptr<Texture> SharedTexture;

  std::string name;
  SharedTexture diffuse;
  SharedTexture specular;
  Material *material;
  std::vector<Mesh *> mesh;

public:
  static std::vector<std::unique_ptr<Object>> from_file(
      std::string file_name,
      std::unique_ptr<TextureManager> &manager,
      std::unique_ptr<MaterialManager> &mat_manager);

  Object(
      std::string name,
      SharedTexture diffuse,
      SharedTexture specular,
      Material *material,
      Mesh *mesh) : name(name),
                    diffuse(diffuse),
                    specular(specular),
                    material(material)
  {
    add_mesh(mesh);
  }
  Object(
      std::string name,
      SharedTexture diffuse,
      SharedTexture specular,
      Material *material) : name(name),
                            diffuse(diffuse),
                            specular(specular),
                            material(material)
  {
  }

  ~Object()
  {
    delete this->material;

    for (std::vector<Mesh *>::iterator it = this->mesh.begin(); it != this->mesh.end(); ++it)
    {
      delete (*it);
    }
    this->mesh.clear();
#ifdef GRA_DEBUG
    Log::log(name + ":" + "destructor");
#endif // DEBUG
  }

  bool has_textures();
  void draw(Shaders *program);
  void clear();
  void initialize();

  inline void add_mesh(Mesh *mesh)
  {
    this->mesh.push_back(mesh);
  }

  inline void set_mesh(std::vector<Mesh *> mesh)
  {
    this->mesh = mesh;
  }
};

#endif // OBJECT_H