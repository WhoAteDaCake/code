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

  for (Mesh *mesh : meshes)
  {
    mesh->auto_normals = true;
    // TODO texture loading ?
    std::shared_ptr<Material> material = mat_manager->get(mesh->mat_name);
    std::shared_ptr<Texture> diffuse_tex = nullptr;

    if (material->diffuse_tex_name.size() != 0)
    {
      diffuse_tex = tex_manager->get(material->diffuse_tex_name);
    }

    std::unique_ptr<Object> object(new Object(
        mesh->get_name() + "_" + mesh->mat_name + "_object",
        diffuse_tex,
        nullptr,
        material, mesh));

    output.push_back(std::move(object));
  }
  return output;
}

bool Object::has_specular()
{
  return this->specular != nullptr;
}

bool Object::has_diffuse()
{
  return this->diffuse != nullptr;
}

void Object::update_position(glm::vec3 position)
{
  for (int i = 0; i < this->mesh.size(); i += 1)
  {
    Mesh *mesh = this->mesh[i];
    if (mesh->dependency_index == -1)
    {
      mesh->position = position;
    }
  }
  update_matrices(false);
}

void Object::update_matrices(bool initial)
{
  std::vector<glm::mat4> matrices;
  glm::mat4 matrix;
  for (int i = 0; i < this->mesh.size(); i += 1)
  {
    Mesh *mesh = this->mesh[i];
    if (mesh->dependency_index == -1)
    {
      matrix = glm::mat4(1.f);
    }
    else
    {
      matrix = matrices[mesh->dependency_index];
    }
    if (initial)
    {

      mesh->initialize(matrix);
    }
    else
    {
      mesh->update(matrix);
    }
    matrices.push_back(mesh->get_model_matrix());
  }
}

void Object::initialize()
{
  update_matrices(true);
  GLint spec_id = has_specular() ? this->specular->get_unit() : -1;
  GLint diff_id = has_diffuse() ? this->diffuse->get_unit() : -1;
  this->material->set_textures(diff_id, spec_id);
#ifdef GRA_DEBUG
  Log::check_error("Object:" + this->name + ":initialize");
#endif
}

void Object::draw(Shaders *program)
{

  this->material->send_to_shader(program);

  if (has_specular())
  {
    this->specular->bind();
  }
  if (has_diffuse())
  {
    this->diffuse->bind();
  }
  for (int i = 0; i < this->mesh.size(); i += 1)
  {
    program->use();
    Mesh *mesh = this->mesh[i];
    mesh->draw(program);
  }
}

void Object::clear()
{
  if (has_specular())
  {
    this->specular->unbind();
  }
  if (has_diffuse())
  {
    this->diffuse->unbind();
  }
}

void Object::update(int delta)
{
}