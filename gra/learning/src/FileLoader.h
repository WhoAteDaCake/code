#pragma once

#ifndef FILE_LOADER_H
#define FILE_LOADER_H

#include <string>
#include <memory>
#include "TextureManager.h"
#include <stdio.h>

class FileLoader
{
public:
  static std::string BASE_DIR;
  static void load(std::string file_name, std::unique_ptr<TextureManager> &manager);
};

#endif // !1 FILE_LOADER_H