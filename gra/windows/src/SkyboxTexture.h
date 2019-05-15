#pragma once

#ifndef SKYBOX_TEXTURE_H
#define SKYBOX_TEXTURE_H

#include "TextureManager.h"

/**
 * Loads in cubemap textures
 */
class SkyboxTexture : public Texture
{
public:
  SkyboxTexture(std::string name) : Texture(GL_TEXTURE_CUBE_MAP, TextureManager::get_id(), name){};

  void load();
  /**
   * Creates a cubemap texture and the faces in order
   */
  GLuint load_cubemap(const char *names[6]);
};

#endif // !YBOX_TEXTURE_H