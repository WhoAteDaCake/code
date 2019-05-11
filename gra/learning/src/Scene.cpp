#include "Scene.h"

Scene::Scene()
{
  this->shader = Shaders();
  this->texture_manager = std::unique_ptr<TextureManager>(new TextureManager());
  this->material_manager = std::unique_ptr<MaterialManager>(new MaterialManager());
}

Scene::~Scene() {}

void Scene::set_camera(Camera *camera)
{
  this->camera = camera;
}

void Scene::initialize()
{

  this->light_pos = glm::vec3(0.f, 0.f, 1.f);
  this->shader.set_shaders("vertex_core.glsl", "fragment_core.glsl", "");

  create_textures();
  create_objects();

  for (std::unique_ptr<Object> &item : this->objects)
  {
    item->initialize();
  }

#ifdef GRA_DEBUG
  Log::log("Scene:initialize:end");
#endif // DEBUG
}

void Scene::draw()
{
  // Camera
  shader.useM4fv("view_matrix", this->camera->get_view_matrix());
  shader.useM4fv("projection_matrix", this->camera->get_projection_matrix());
  shader.use3fv("camera_pos", this->camera->position);

  // Lights
  shader.use3fv("light_pos0", this->light_pos);

  // Render items
  for (std::unique_ptr<Object> &item : this->objects)
  {
    item->draw(&this->shader);
  }
#ifdef GRA_DEBUG
  Log::check_error("Scene:draw");
#endif // DEBUG
}

void Scene::clear()
{
  for (std::unique_ptr<Object> &item : this->objects)
  {
    item->clear();
  }
}

void Scene::create_textures()
{
  this->texture_manager->add("wall.png", GL_TEXTURE_2D);
  // this->texture_manager->add("pusheen_specular.png", GL_TEXTURE_2D);
}

void Scene::create_objects()
{
  std::vector<std::unique_ptr<Object>> items = Object::from_file(std::string("room-high.obj"), this->texture_manager, this->material_manager);
  std::move(std::begin(items), std::end(items), std::back_inserter(this->objects));

  float limit = 255.f;
  auto pink = glm::vec3(229.f / limit, 137.f / limit, 135.f / limit);

  auto pig = make_pig(
      "pig",
      this->material_manager,
      pink,
      glm::vec3(0.f, 0.f, 0.f),
      glm::vec3(0.f, 90.f, 0.f),
      glm::vec3(1.f));

  auto blue = glm::vec3(17.f / limit, 41.f / limit, 77.f / limit);
  auto right_wall = make_cube(
      std::string("right-wall"),
      this->material_manager,
      blue,
      // Position
      glm::vec3(-10.f, 33.5f, -69.f),
      glm::vec3(0.f, 0.f, 0.f),
      // Scale
      glm::vec3(51.f, 35.f, 2.f));
  auto back_wall = make_cube(
      std::string("back-wall"),
      this->material_manager,
      blue,
      // Position
      glm::vec3(40.5f, 30.f, -15.f),
      glm::vec3(0.f, 0.f, 0.f),
      // Scale
      glm::vec3(1.f, 35.f, 51.f));

  auto roof = make_cube(
      std::string("roof"),
      this->material_manager,
      blue,
      // Position
      glm::vec3(-10.5f, 65.5f, -16.f),
      glm::vec3(0.f, 0.f, 0.f),
      // Scale
      glm::vec3(51.f, 2.f, 51.f));

  auto wall_tex = this->texture_manager->get("wall.png");
  right_wall->set_diffuse(wall_tex);
  back_wall->set_diffuse(wall_tex);

  this->objects.push_back(std::move(pig));
  this->objects.push_back(std::move(right_wall));
  this->objects.push_back(std::move(roof));
  this->objects.push_back(std::move(back_wall));
}