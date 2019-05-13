#include "Scene.h"
#include <memory>

Scene::Scene()
{
  // TMP
  // this->light.diffuse = glm::vec3(1.f, 1.f, 1.f);
  // this->light.position = glm::vec3(-10.f, 30.f, -15.4f);
  // this->light.direction = glm::vec3(0.f, -1.f, 0.f);
  // this->light.linear = 0.007f;
  // this->light.constant = 0.0002f;
  // this->light.specular = glm::vec3(1.f, 1.f, 1.f);
  // this->light.ambient = glm::vec3(0.8f);
  // this->light.cut_off = 30.f;
  // this->light.outer_cut_off = 90.f;
  // this->light.type = 2;

  this->shader = Shaders();
  this->sky_shader = SkyboxShaders();
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
  this->shader.set_shaders("core.vs.glsl", "core.fs.glsl", "");

  // Skybox
  this->sky_mesh = std::unique_ptr<SkyboxMesh>(new SkyboxMesh("skybox_mesh"));
  this->sky_tex = std::unique_ptr<SkyboxTexture>(new SkyboxTexture("skybox_texture"));
  this->sky_mesh->initialize(glm::mat4(1.f));
  this->sky_tex->load();
  this->sky_shader.set_shaders("sky.vs.glsl", "sky.fs.glsl", "");

  create_textures();
  create_lights();
  create_objects();

  for (std::unique_ptr<Object> &item : this->objects)
  {
    item->initialize();
  }

  // Lights
  for (auto &light : this->lights)
  {
    light->initialize();
  }

#ifdef GRA_DEBUG
  Log::log("Scene:initialize:end");
#endif // DEBUG
}

void Scene::draw()
{
  // START_OF_SKYBOX
  this->sky_shader.use();
  this->sky_tex->bind();
  this->sky_shader.use1i("skybox", this->sky_tex->get_unit());

#ifdef GRA_DEBUG
  Log::check_error("Scene:skybox:texture");
#endif // DEBUG

  // remove translation from the view matrix
  glm::mat4 view = glm::mat4(glm::mat3(this->camera->get_view_matrix()));
  this->sky_shader.useM4fv("view", view);
  this->sky_shader.useM4fv("projection", this->camera->get_projection_matrix());
#ifdef GRA_DEBUG
  Log::check_error("Scene:skybox:projection");
#endif // DEBUG

  this->sky_shader.use();
  this->sky_mesh->draw(&this->sky_shader);
  this->sky_shader.stop_use();
#ifdef GRA_DEBUG
  Log::check_error("Scene:skybox:draw");
#endif // DEBUG
  // END_OF_SKYBOX

  // Camera

  shader.useM4fv("view_matrix", this->camera->get_view_matrix());
  shader.useM4fv("projection_matrix", this->camera->get_projection_matrix());
  shader.use3fv("camera_pos", this->camera->position);

  // Lights
  for (auto &light : this->lights)
  {
    light->draw(&this->shader);
  }
  // light.send_to_shader(&this->shader);

  // Render items
  for (std::unique_ptr<Object> &item : this->objects)
  {
    item->draw(&this->shader);
  }
  shader.stop_use();
#ifdef GRA_DEBUG
  Log::check_error("Scene:draw");
#endif // DEBUG
}

void Scene::clear()
{
  for (auto &item : this->objects)
  {
    item->clear();
  }

  for (auto &item : this->lights)
  {
    item->clear();
  }

  this->sky_tex->unbind();
}

void Scene::create_textures()
{
  this->texture_manager->add("wall.png", GL_TEXTURE_2D);
  // this->texture_manager->add("pusheen_specular.png", GL_TEXTURE_2D);
}

void Scene::create_lights()
{
  // Lights
  auto cube1 = make_light_cube(
      "outside",
      this->material_manager,
      glm::vec3(1.f),
      glm::vec3(-45.f, 47.f, 15.f),
      glm::vec3(0.f),
      glm::vec3(3.f));

  cube1->light->type = 1;
  cube1->light->direction = glm::vec3(0.f, -1.f, 0.f);
  cube1->light->specular = glm::vec3(0.1f);
  cube1->light->diffuse = glm::vec3(1.f);
  cube1->light->linear = 0.0014;
  cube1->light->constant = 0.00007;

  auto cube2 = make_light_cube(
      "outside",
      this->material_manager,
      glm::vec3(1.f),
      glm::vec3(-43.9f, 47.f, -49.f),
      glm::vec3(0.f),
      glm::vec3(3.f));

  cube2->light->type = 1;
  cube2->light->direction = glm::vec3(0.f, -1.f, 0.f);
  cube2->light->specular = glm::vec3(0.1f);
  cube2->light->diffuse = glm::vec3(1.f);
  cube2->light->linear = 0.0014;
  cube2->light->constant = 0.00007;

  auto fan = make_fan_lamp(
      "outside",
      this->material_manager,
      glm::vec3(1.f),
      // Position
      glm::vec3(-20.f, 60.f, -20.f),
      glm::vec3(0.f),
      // Scale
      glm::vec3(6.f));

  fan->light->type = 2;
  fan->light->linear = 0.0014;
  fan->light->constant = 0.00007;
  fan->light->diffuse = glm::vec3(1.f);
  fan->light->direction = glm::vec3(0.f, -1.f, 0.f);
  fan->light->specular = glm::vec3(0.2f);
  fan->light->ambient = glm::vec3(0.8f);

  this->lights.push_back(std::move(fan));
  this->lights.push_back(std::move(cube1));
  this->lights.push_back(std::move(cube2));
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
      // Postion
      glm::vec3(-30.f, 9.f, -27.4f),
      // Rotation
      glm::vec3(0.f, -90.f, 0.f),
      // Scale
      glm::vec3(3.f));

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

  // auto wall_tex = this->texture_manager->get("wall.png");
  // right_wall->set_diffuse(wall_tex);
  // back_wall->set_diffuse(wall_tex);

  this->objects.push_back(std::move(pig));
  this->objects.push_back(std::move(right_wall));
  this->objects.push_back(std::move(roof));
  this->objects.push_back(std::move(back_wall));
}

void Scene::update(int delta)
{
  // Update
  for (std::unique_ptr<Object> &item : this->objects)
  {
    item->update(delta);
  }
  for (auto &item : this->lights)
  {
    item->update(delta);
  }
}