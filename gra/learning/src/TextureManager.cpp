#include "TextureManager.h"

unsigned int TextureManager::ID = 0;

unsigned int TextureManager::get_id()
{
  auto id = TextureManager::ID;
  TextureManager::ID += 1;
  return id;
}