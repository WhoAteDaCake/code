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

/**
 * Container class used to store and group
 * meshes and material
 */
class Object
{
protected:
  typedef std::shared_ptr<Texture> SharedTexture;

  std::string name;
  /**
   * Textures that are assigned to the material
   */
  SharedTexture diffuse;
  SharedTexture specular;
  /**
   * Material definitions, that all of the meshes
   * will use
   */
  std::shared_ptr<Material> material;
  /**
   * Grouped mesh
   */
  std::vector<Mesh *> mesh;

public:
  /**
   * Load the whole object from an .obj file 
   */
  static std::vector<std::unique_ptr<Object>> from_file(
      std::string file_name,
      std::unique_ptr<TextureManager> &tex_manager,
      std::unique_ptr<MaterialManager> &mat_manager);

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
  /**
   * Set materials, textures
   * and draw meshes to the screen
   */
  void draw(Shaders *program);
  /**
   * Unbind textures
   */
  void clear();
  /**
   * Recalculate model matrix of each mesh
   * Using dependencies to create hierarchical models
   */
  void update_matrices(bool initial);
  /**
   * Change where initial mesh is placed
   * Combined with dependency indices allows
   * to move the whole object
   */
  void update_position(glm::vec3 position);
  /**
   * Change the scale of the initial mesh
   * Combined with dependency indices allows
   * to move the whole object
   */
  void update_scale(glm::vec3 scale);
  /**
   * Initialize meshes, and materials 
   */
  virtual void initialize();
  /**
   * Update for animations, usually defined by superclass
   */
  virtual void update(int delta);

  inline std::shared_ptr<Material> get_material()
  {
    return this->material;
  }

  inline glm::vec3 get_position()
  {
    return this->mesh[0]->position;
  }

  /**
   * Sets the diffuse texture
   */
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