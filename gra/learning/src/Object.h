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
protected:
  typedef std::shared_ptr<Texture> SharedTexture;

  std::string name;
  SharedTexture diffuse;
  SharedTexture specular;
  std::shared_ptr<Material> material;
  std::vector<Mesh *> mesh;

public:
  static std::vector<std::unique_ptr<Object>> from_file(
      std::string file_name,
      std::unique_ptr<TextureManager> &tex_manager,
      std::unique_ptr<MaterialManager> &mat_manager);

  std::shared_ptr<Material> secondary_mat;

  Object(
      std::string name,
      SharedTexture diffuse,
      SharedTexture specular,
      std::shared_ptr<Material> material,
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
      std::shared_ptr<Material> material) : name(name),
                                            diffuse(diffuse),
                                            specular(specular),
                                            material(material)
  {
  }

  ~Object()
  {

    for (std::vector<Mesh *>::iterator it = this->mesh.begin(); it != this->mesh.end(); ++it)
    {
      delete (*it);
    }
    this->mesh.clear();
#ifdef GRA_DEBUG
    Log::log(name + ":" + "destructor");
#endif // DEBUG
  }

  bool has_diffuse();
  bool has_specular();
  void draw(Shaders *program);
  void clear();
  void update_matrices(bool initial);
  void update_position(glm::vec3 position);

  virtual void initialize();
  virtual void update(int delta);

  inline glm::vec3 get_position()
  {
    return this->mesh[0]->position;
  }

  inline void set_diffuse(SharedTexture diffuse_tex)
  {
    this->diffuse = diffuse_tex;
    GLint spec_id = has_specular() ? this->specular->get_unit() : -1;
    GLint diff_id = has_diffuse() ? this->diffuse->get_unit() : -1;
    this->material->set_textures(diff_id, spec_id);
  }

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