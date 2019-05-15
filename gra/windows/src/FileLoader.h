#pragma once

#ifndef FILE_LOADER_H
#define FILE_LOADER_H

#include <string>
#include <memory>
#include <stdio.h>
#include "TextureManager.h"
#include "MaterialManager.h"
#include "Vertex.h"
#include "Mesh.h"

/**
 * Uses tinyobjloader to load:
 * 	Mmaterials 
 * 	Textures,
 * 	Meshes
 * From a give file
 */
class FileLoader
{
public:
	/**
	 * Location where the exports are stored
	 */
  static std::string BASE_DIR;
  static std::vector<Mesh *> load(
      std::string file_name,
      std::unique_ptr<TextureManager> &tex_manager,
      std::unique_ptr<MaterialManager> &mat_manager);
};

#endif // !1 FILE_LOADER_H