#include "Mesh.h"

void Mesh::set_pointers()
{
  // SET VERTEXATTRIBPOINTERS AND ENABLE (INPUT ASSEMBLY)
  // Position
  // Could use glGetAtrributeLocation
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (GLvoid *)offsetof(Vertex, position));
  glEnableVertexAttribArray(0);
  // Color
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (GLvoid *)offsetof(Vertex, color));
  glEnableVertexAttribArray(1);
  // Texture coord
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (GLvoid *)offsetof(Vertex, texcoord));
  glEnableVertexAttribArray(2);
  // Normals
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (GLvoid *)offsetof(Vertex, normal));
  glEnableVertexAttribArray(3);
}

void Mesh::bind_buffers()
{
  // GEN VAO AND BIND
  glGenVertexArrays(1, &this->VAO);
  glBindVertexArray(this->VAO);

  // //GEN VBO AND BIND AND SEND DATA
  glGenBuffers(1, &this->VBO);
  glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
  glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), vertices.data(), this->draw_type);

  // //GEN EBO AND BIND AND SEND DATA
  if (has_indices())
  {
    glGenBuffers(1, &this->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), indices.data(), this->draw_type);
  }
  set_pointers();
  // BIND VAO 0
  glBindVertexArray(0);
}

void Mesh::calculate_matrix(glm::mat4 initial_matrix)
{
  this->model_matrix = initial_matrix;
  this->model_matrix = glm::translate(this->model_matrix, this->position);
  this->model_matrix = glm::rotate(this->model_matrix, glm::radians(this->rotation.x), glm::vec3(1.0f, 0.f, 0.f));
  this->model_matrix = glm::rotate(this->model_matrix, glm::radians(this->rotation.y), glm::vec3(0.0f, 1.f, 0.f));
  this->model_matrix = glm::rotate(this->model_matrix, glm::radians(this->rotation.z), glm::vec3(0.0f, 0.f, 1.f));
  this->model_matrix = glm::scale(this->model_matrix, this->scale);
}

void Mesh::calculate_normals()
{
  // At the moment only do normals for vertex only mesh
  if (this->indices.size() != 0)
  {
    return;
  }
  for (int i = 0; i < this->vertices.size(); i += 3)
  {
    glm::vec3 normal = glm::triangleNormal(
        this->vertices[i + 0].position,
        this->vertices[i + 1].position,
        this->vertices[i + 2].position);
    for (int k = 0; k < 3; k += 1)
    {
      this->vertices[i + k].normal = normal;
    }
  }
}

void Mesh::initialize(glm::mat4 initial_matrix)
{
  if (this->vertices.empty())
  {
    throw message("Has not initialized vertices");
  }
  calculate_normals();
  bind_buffers();
  calculate_matrix(initial_matrix);
#ifdef GRA_DEBUG
  Log::log(message("initialize"));
#endif // DEBUG
}

void Mesh::draw(Shaders *program)
{

  // Make sure matrix gets injected
  program->useM4fv("model_matrix", this->model_matrix);

  program->use();

  glBindVertexArray(this->VAO);

  if (!has_indices())
  {
    glDrawArrays(GL_TRIANGLES, 0, this->vertices.size());
  }
  else
  {
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
  }
}

void Mesh::update(glm::mat4 initial_matrix)
{
  calculate_matrix(initial_matrix);
}