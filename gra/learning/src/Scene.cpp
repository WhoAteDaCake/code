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
  // this->texture_manager->add("pusheen.png", GL_TEXTURE_2D);
  // this->texture_manager->add("pusheen_specular.png", GL_TEXTURE_2D);
}

void Scene::create_objects()
{
  // Square *mySquare = new Square("test-square", 0.5f);
  // Material *material = new Material(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f));
  // material->toggle_color(false);

  std::unique_ptr<Object> testObj(Object::from_file(std::string("room.obj"), this->texture_manager));
  // std::unique_ptr<Object> testObj(new Object(
  //     "test-obj",
  //     // nullptr,
  //     // nullptr,
  //     this->texture_manager->get("pusheen.png"),
  //     this->texture_manager->get("pusheen_specular.png"),
  //     material,
  //     mySquare));

  this->objects.push_back(std::move(testObj));
}