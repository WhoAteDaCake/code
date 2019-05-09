#include "Object.h"

// #define TINYOBJLOADER_IMPLEMENTATION
// #include "tiny_obj_loader.h"

Object *Object::from_file(std::string file_name, std::unique_ptr<TextureManager> &manager)
{
  std::vector<Mesh *> meshes = FileLoader::load(file_name, manager);

  // Square *mySquare = new Square("test-square", 0.5f);
  Material *material = new Material(glm::vec3(0.5f), glm::vec3(1.f), glm::vec3(1.f));

  Object *object = new Object(
      file_name + "_object",
      nullptr,
      nullptr,
      material);
  object->set_mesh(meshes);

  return object;
}

bool Object::has_textures()
{
  return this->diffuse != nullptr && this->specular != nullptr;
}

void Object::initialize()
{
  for (int i = 0; i < this->mesh.size(); i += 1)
  {
    Mesh *mesh = this->mesh[i];
    mesh->initialize();
  }
  if (has_textures())
  {
    this->material->set_textures(this->diffuse->get_unit(), this->specular->get_unit());
  }

#ifdef GRA_DEBUG
  Log::check_error("Object:" + this->name + ":initialize");
#endif
}

void Object::draw(Shaders *program)
{

  this->material->send_to_shader(program);

  program->use();
  if (has_textures())
  {
    this->diffuse->bind();
    this->specular->bind();
  }
  for (int i = 0; i < this->mesh.size(); i += 1)
  {
    Mesh *mesh = this->mesh[i];
    mesh->draw(program);
  }
}

void Object::clear()
{
  if (has_textures())
  {
    this->diffuse->unbind();
    this->specular->unbind();
  }
}