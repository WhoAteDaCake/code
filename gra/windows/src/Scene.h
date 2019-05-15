#pragma once

#ifndef SCENE_H
#define SCENE_H

#include "Camera.h"
#include "Object.h"
#include "Shapes.h"
#include "TextureManager.h"
#include "MaterialManager.h"
#include "CommonObjects.h"
#include "SkyboxShaders.h"
#include "SkyboxTexture.h"
#include "Light.h"

class Scene
{
private:
  // Camera used by the scene
  Camera *camera;
  // All of the lights used in the scene
  std::vector<std::unique_ptr<ObjectLight>> lights;

  Shaders shader;
  /** Skybox properties */
  std::unique_ptr<SkyboxTexture> sky_tex;
  std::unique_ptr<SkyboxMesh> sky_mesh;
  SkyboxShaders sky_shader;

  /** All of the data drawn in the scene */
  std::vector<std::unique_ptr<Object>> objects;
  std::unique_ptr<TextureManager> texture_manager;
  std::unique_ptr<MaterialManager> material_manager;

  /** Initiations of the scene */
  void create_objects();
  void create_textures();
  void create_lights();

public:
  Scene();
  ~Scene();

  // Can't allow to be coppied becayse of unique_pts
  Scene &operator=(const Scene &) = delete; // non copyable
  Scene(const Scene &) = delete;            // non construction-copyable

  /** Assign a camera */
  void set_camera(Camera *camera);
  /** Draw the scene */
  void draw();
  /** Call initialization functions */
  void initialize();
  /** Cleanup the scene */
  void clear();
  /** Update the objects within the scene */
  void update(int delta);
};

#endif