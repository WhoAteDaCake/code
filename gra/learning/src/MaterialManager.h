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

  void add(std::string name, glm::vec3 ambient,
           glm::vec3 diffuse,
           glm::vec3 specular)
  {
    std::shared_ptr<Material> texture = std::make_shared<Material>(ambient, diffuse, specular);
    this->items.insert(std::pair<std::string, std::shared_ptr<Material>>(name, std::move(texture)));
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