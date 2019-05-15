#pragma once

#include "Texture.h"
#include <memory>
#include <map>

#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

class TextureManager
{
private:
  std::map<std::string, std::shared_ptr<Texture>> textures;

public:
  /**
   * Makes sure each texture has a unique id
   * When registering in openGL context
   */
  static unsigned int ID;
  static unsigned int get_id();

  TextureManager(){};
  ~TextureManager() {}

  /** Create new texture */
  void add(std::string name, GLenum type)
  {
#ifdef GRA_DEBUG
    Log::log(std::string("TextureManger:add:") + std::to_string(TextureManager::ID) + ":" + name);
#endif // DEBUG

    std::shared_ptr<Texture> texture = std::make_shared<Texture>(type, TextureManager::ID, name);
    texture->load();
    this->textures.insert(std::pair<std::string, std::shared_ptr<Texture>>(name, std::move(texture)));
    TextureManager::ID += 1;
  }

  std::shared_ptr<Texture> get(std::string name)
  {
    auto it = textures.find(name);
    if (it == textures.end())
    {
      throw std::string("Could not find texture: " + name);
    }
    return it->second;
  }
};

#endif // DEBUG