#include "Mesh.h"

void Mesh::bind_buffers()
{
  // GEN VAO AND BIND
  glGenVertexArrays(1, &this->VAO);
  glBindVertexArray(this->VAO);

  // //GEN VBO AND BIND AND SEND DATA
  glGenBuffers(1, &this->VBO);
  glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
  // GL_STATIC_DRAW because we won't modify the values
  glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), vertices.data(), this->draw_type);

  // //GEN EBO AND BIND AND SEND DATA
  glGenBuffers(1, &this->EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), indices.data(), this->draw_type);

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

  // BIND VAO 0
  glBindVertexArray(0);
}

void Mesh::calculate_matrix()
{
  this->model_matrix = glm::mat4(1.f);
  this->model_matrix = glm::translate(this->model_matrix, this->position);
  this->model_matrix = glm::rotate(this->model_matrix, glm::radians(this->rotation.x), glm::vec3(1.0f, 0.f, 0.f));
  this->model_matrix = glm::rotate(this->model_matrix, glm::radians(this->rotation.y), glm::vec3(0.0f, 1.f, 0.f));
  this->model_matrix = glm::rotate(this->model_matrix, glm::radians(this->rotation.z), glm::vec3(0.0f, 0.f, 1.f));
  this->model_matrix = glm::scale(this->model_matrix, this->scale);
}

void Mesh::initialize()
{
  if (this->vertices.empty() || this->indices.empty())
  {
    throw error_msg("Has not initialized vertices and indices");
  }
  bind_buffers();
  calculate_matrix();
}

void Mesh::update_uniforms(Shaders *program)
{
  program->useM4fv("model_matrix", this->model_matrix);
}

void Mesh::render(Shaders *program)
{
  // Make sure matrix gets injected
  this->update_uniforms(program);

  program->use();

  glBindVertexArray(this->VAO);
  glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
}

void Mesh::update()
{
}