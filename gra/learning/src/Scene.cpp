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
  this->shader.set_shaders("vertex_core.glsl", "fragment_core.glsl", "");
  // TMP
  // this->square.initialize();
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

  // square.render(&shader);
}