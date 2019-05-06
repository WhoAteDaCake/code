#include "Scene.h"

Scene::Scene()
{
  this->shader = Shaders();
  this->texture_manager = std::unique_ptr<TextureManager>(new TextureManager());
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

  this->texture_manager->add("pusheen.png", GL_TEXTURE_2D);
  this->texture_manager->add("container.png", GL_TEXTURE_2D);

  Square *mySquare = new Square("test-square", 0.5f);
  // Texture *diffuse = new Texture(GL_TEXTURE_2D, 0, "pusheen.png");
  // Texture *specular = new Texture(GL_TEXTURE_2D, 1, "container.png");
  Material *material = new Material(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f));

  std::unique_ptr<Object> testObj(new Object(
      "test-obj",
      this->texture_manager->get("pusheen.png"),
      this->texture_manager->get("container.png"),
      material,
      mySquare));

  this->objects.push_back(std::move(testObj));

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