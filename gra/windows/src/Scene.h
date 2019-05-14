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
  // TODO:CHANGE TO private
public:
  Camera *camera;
  std::vector<std::unique_ptr<ObjectLight>> lights;

  Shaders shader;
  // Skybox
  std::unique_ptr<SkyboxTexture> sky_tex;
  std::unique_ptr<SkyboxMesh> sky_mesh;
  SkyboxShaders sky_shader;

  std::vector<std::unique_ptr<Object>> objects;
  std::unique_ptr<TextureManager> texture_manager;
  std::unique_ptr<MaterialManager> material_manager;

  // Initiations
  void create_objects();
  void create_textures();
  void create_lights();

public:
  Scene();
  ~Scene();

  // Can't allow to be coppied becayse of unique_pts
  Scene &operator=(const Scene &) = delete; // non copyable
  Scene(const Scene &) = delete;            // non construction-copyable

  void set_camera(Camera *camera);
  void draw();
  void initialize();
  void clear();
  void update(int delta);
};

#endif