#pragma once

#include "Material.h"
#include <memory>
#include <map>

#ifndef MATERIAL_MANAGER_H
#define MATERIAL_MANAGER_H

class MaterialManager
{
private:
  std::map<std::string, std::shared_ptr<Material>> items;

public:
  static unsigned int ID;
  MaterialManager(){};
  ~MaterialManager() {}

  void add(std::string name,
           glm::vec3 ambient,
           glm::vec3 diffuse,
           glm::vec3 specular,
           bool show_color)
  {
    std::shared_ptr<Material> item = std::make_shared<Material>(ambient, diffuse, specular, show_color);

    this->items.insert(std::make_pair(
        name,
        std::move(item)));
    MaterialManager::ID += 1;
  }

  std::shared_ptr<Material> get(std::string name)
  {
    auto it = items.find(name);
    if (it == items.end())
    {
      throw std::string("Could not find texture: " + name);
    }
    return it->second;
  }
};

#endif // DEBUG