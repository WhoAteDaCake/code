#pragma once

#ifndef FILE_LOADER_H
#define FILE_LOADER_H

#include <string>
#include <memory>
#include <stdio.h>
#include "TextureManager.h"
#include "Vertex.h"
#include "Mesh.h"

class FileLoader
{
public:
  static std::string BASE_DIR;
  static std::vector<Mesh *> load(std::string file_name, std::unique_ptr<TextureManager> &manager, std::vector<std::shared_ptr<Material>> &materials);
};

#endif // !1 FILE_LOADER_H