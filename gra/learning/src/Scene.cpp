#include "Scene.h"

Scene::Scene()
{
  this->shader = Shaders();
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

  Square *mySquare = new Square("test-square", 0.5f);

  Object testObj = Object(
      "test-obj",
      Texture(GL_TEXTURE_2D, 0, "pusheen.png"),
      Texture(GL_TEXTURE_2D, 1, "container.png"),
      Material(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f)),
      mySquare);

  this->objects.push_back(testObj);

  for (Object &item : this->objects)
  {
    item.initialize();
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
  for (Object &item : this->objects)
  {
    item.draw(&this->shader);
  }
#ifdef GRA_DEBUG
  Log::check_error("Scene:draw");
#endif // DEBUG
}

void Scene::clear()
{
  for (Object &item : this->objects)
  {
    item.clear();
  }
}