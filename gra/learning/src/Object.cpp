#include "Object.h"

// #define TINYOBJLOADER_IMPLEMENTATION
// #include "tiny_obj_loader.h"

std::vector<std::unique_ptr<Object>> Object::from_file(
    std::string file_name,
    std::unique_ptr<TextureManager> &tex_manager,
    std::unique_ptr<MaterialManager> &mat_manager)
{
  std::vector<std::unique_ptr<Object>> output;
  std::vector<Mesh *> meshes = FileLoader::load(file_name, tex_manager, mat_manager);

  // for (Mesh *mesh : meshes)
  // {
  //   Material *material = new Material(glm::vec3(0.5f), glm::vec3(1.f), glm::vec3(1.f));
  //   if (mesh->texture_name.size() != 0)
  //   {
  //     material->set_textures(manager->get(mesh->texture_name)->get_unit(), (GLint)-1);
  //     material->toggle_color(false);
  //   }

  //   std::unique_ptr<Object> object(new Object(
  //       mesh->get_name() + "_object",
  //       nullptr,
  //       nullptr,
  //       material, mesh));

  //   output.push_back(std::move(object));
  // }
  // // Square *mySquare = new Square("test-square", 0.5f);
  return output;
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