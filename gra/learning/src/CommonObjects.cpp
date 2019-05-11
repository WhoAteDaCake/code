#define loop3(x) for (int x = 0; x < 3; ++x)
#define revLoop3(x) for (int x = 2; x > -1; --x)

#include "CommonObjects.h"

Mesh *make_cube_mesh(std::string name, glm::vec3 color, float size)
{
  auto make_vertex = [&, color](float f1, float f2, float f3, float t1, float t2) { return Vertex(glm::vec3(f1, f2, f3), color, glm::vec2(t1, t2), glm::vec3(0.f)); };

  // Will require 8 corners
  Vertex vertices[] = {
      // Front
      make_vertex(-size, size, size, 0.f, 0.f),
      make_vertex(-size, -size, size, 0.f, size),
      make_vertex(size, -size, size, size, size),
      make_vertex(size, size, size, size, 0.f),
      // Back
      make_vertex(-size, size, -size, size, 0.f),
      make_vertex(-size, -size, -size, 0, size),
      make_vertex(size, -size, -size, size, size),
      make_vertex(size, size, -size, 0.f, 0.f)};

  glm::vec3 normals[] = {
      glm::vec3(0.f, 0.f, 1.f),  // F
      glm::vec3(1.f, 0.f, 0.f),  // R
      glm::vec3(0.f, 0.f, -1.f), // B
      glm::vec3(-1.f, 0.f, 0.f), // L
      glm::vec3(0.f, 1.f, 0.f),  // T
      glm::vec3(0.f, -1.f, 0.f), // Bot
  };
  GLuint indices[] = {
      // FRONT
      0, 1, 2,
      0, 2, 3,
      // RIGHT
      3, 2, 6,
      3, 6, 7,
      // BACK
      7, 6, 5,
      4, 7, 5,
      // LEFT
      0, 4, 5,
      0, 5, 1,
      // TOP
      7, 4, 0,
      7, 0, 3,
      // BOTTOM
      6, 2, 1,
      6, 1, 5};

  // We dont use idices here because it's easier to calculate normals this way
  Mesh *mesh = new Mesh(name);
  for (int i = 0; i < sizeof(indices) / sizeof(GLuint); i += 3)
  {
    int normal_i = floor((i / 3) / 2);
    // Calculate normals
    Vertex faces[3];
    loop3(k)
    {
      faces[k] = vertices[indices[i + k]];
    }
    // glm::vec3 normal = glm::triangleNormal(faces[0].position, faces[1].position, faces[2].position);
    loop3(k)
    {
      faces[k].normal = normals[normal_i];
      mesh->vertices.push_back(faces[k]);
    }
  }
  return mesh;
}

std::unique_ptr<Object> make_cube(
    std::string name,
    std::unique_ptr<MaterialManager> &mat_manager,
    glm::vec3 color,
    glm::vec3 position,
    glm::vec3 rotation,
    glm::vec3 scale)
{

  Mesh *mesh = make_cube_mesh(name, color, 1.f);
  mesh->position = position;
  mesh->scale = scale;
  mesh->rotation = rotation;

  std::shared_ptr<Material> material = std::make_shared<Material>(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f), true);
  return std::unique_ptr<Object>(new Object(name, nullptr, nullptr, material, mesh));
}

std::unique_ptr<Object> make_pig(
    std::string name,
    std::unique_ptr<MaterialManager> &mat_manager,
    glm::vec3 color,
    glm::vec3 position,
    glm::vec3 rotation,
    glm::vec3 scale)
{
  float size = 1.f;
  std::shared_ptr<Material> material = std::make_shared<Material>(glm::vec3(1.f), glm::vec3(1.f), glm::vec3(1.f), true);
  Object *object = new Object(name, nullptr, nullptr, material);
  // Body
  Mesh *body = make_cube_mesh(name + "_body", color, size);
  body->position = position;
  body->scale = scale * glm::vec3(2.f, 1.f, 1.f);
  body->rotation = rotation;
  // Head
  Mesh *head = make_cube_mesh(name + "_head", color, size);
  head->position = body->position + glm::vec3(body->scale.x, size * 0.5, 0.f);
  head->scale = scale * glm::vec3(0.8f);
  head->rotation = rotation;
  // Leg 1
  Mesh *leg1 = make_cube_mesh(name + "_leg1", color, size);
  leg1->position = position + glm::vec3(0.f, -size * 2, 0.f);
  leg1->scale = scale * glm::vec3(0.3f, 0.8f, 0.3f);
  leg1->rotation = rotation;
  // Leg 2
  // Leg 3
  // Leg 5
  object->add_mesh(body);
  object->add_mesh(head);
  object->add_mesh(leg1);
  return std::unique_ptr<Object>(object);
}