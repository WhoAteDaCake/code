#define loop3(x) for (int x = 0; x < 3; ++x)
#define revLoop3(x) for (int x = 2; x > -1; --x)

#include "CommonObjects.h"

std::unique_ptr<Object> make_cube(
    std::unique_ptr<MaterialManager> &mat_manager,
    glm::vec3 color,
    glm::vec3 position,
    glm::vec3 rotation,
    glm::vec3 scale)
{
  float size = 1.f;
  float start = 0.f;
  auto make_vertex = [&, color](glm::vec3 pos) { return Vertex(pos, color, glm::vec2(0.f), glm::vec3(0.f)); };

  // Will require 8 corners
  Vertex vertices[] = {
      make_vertex(glm::vec3(-size, -size, -size)), // 0
      make_vertex(glm::vec3(-size, -size, size)),  // 1
      make_vertex(glm::vec3(size, -size, size)),   // 2
      make_vertex(glm::vec3(size, -size, -size)),  // 3
      //
      make_vertex(glm::vec3(-size, size, -size)), // 4
      make_vertex(glm::vec3(-size, size, size)),  // 5
      make_vertex(glm::vec3(size, size, size)),   // 6
      make_vertex(glm::vec3(size, size, -size)),  // 7

      // Left side
      // make_vertex(glm::vec3(start, start, start)), // 0
      // make_vertex(glm::vec3(start, size, start)),  // 1
      // make_vertex(glm::vec3(start, size, size)),   // 2
      // make_vertex(glm::vec3(start, start, size)),  // 3
      // // Right side
      // make_vertex(glm::vec3(size, start, start)), // 4
      // make_vertex(glm::vec3(size, size, start)),  // 5
      // make_vertex(glm::vec3(size, size, size)),   // 6
      // make_vertex(glm::vec3(size, start, size)),  // 7
  };

  glm::vec3 normals[] = {
      glm::vec3(1.f, 0.f, 0.f),
      glm::vec3(-1.f, 0.f, 0.f),
      glm::vec3(0.f, 1.f, 0.f),
      glm::vec3(0.f, -1.f, 0.f),
  };

  GLuint indices[] = {
      // TOP
      4,
      5,
      6,
      6,
      7,
      4,
      // BOTTOM
      0,
      3,
      2,
      2,
      1,
      0};
  // We dont use idices here because it's easier to calculate normals this way
  std::vector<Vertex> v_vector;
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
    // Push the vertices
    faces[i + 0].color = glm::vec3(1.f, 0.f, 0.f);
    faces[i + 1].color = glm::vec3(0.f, 1.f, 0.f);
    faces[i + 2].color = glm::vec3(0.f, 0.f, 1.f);
    loop3(k)
    {
      faces[k].normal = normals[normal_i];
      v_vector.push_back(faces[k]);
    }
  }

  Mesh *mesh = new Mesh("Cube");
  mesh->vertices = v_vector;
  mesh->position = position;
  mesh->scale = scale;
  mesh->rotation = rotation;

  std::shared_ptr<Material> material = std::make_shared<Material>(glm::vec3(0.5f), glm::vec3(1.f), glm::vec3(1.f), true);
  return std::unique_ptr<Object>(new Object("Cube_object", nullptr, nullptr, material, mesh));
}