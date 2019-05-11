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
  std::shared_ptr<Material> material = std::make_shared<Material>(glm::vec3(0.3f), glm::vec3(1.f), glm::vec3(1.f), true);
  PigObject *object = new PigObject(name, nullptr, nullptr, material);
  // Body
  float body_scale = 2.f;
  Mesh *body = make_cube_mesh(name + "_body", color, size);
  body->position = position;
  body->scale = scale * glm::vec3(body_scale, 1.f, 1.f);
  body->rotation = rotation;
  // Head
  float head_scale = 0.8f;
  Mesh *head = make_cube_mesh(name + "_head", color, size);
  head->dependency_index = 0;
  head->position = glm::vec3(size * head_scale, size * 0.5, 0.f);
  head->scale = glm::vec3(1 / body_scale, head_scale, head_scale);
  // Leg 1
  float leg_scale = 0.4;
  Mesh *leg1 = make_cube_mesh(name + "_leg1", color, size);
  leg1->dependency_index = 0;
  leg1->position = glm::vec3(
      // Move back so it's aligned with the end of body
      -leg_scale * 2,
      // Move down to align with the bottom
      // *4 because we move down twice:
      // First to half inside the body
      // And then fully outside
      -leg_scale * 4,
      // Move to the right of the body
      -leg_scale * 2 + (leg_scale / 2));
  leg1->scale = glm::vec3(
      // Reset body scale(x) and apply own scale
      (1 / body_scale) * leg_scale,
      // Leg height is twice the width
      leg_scale * 2,
      // Square legs so z is the same
      leg_scale);
  // Leg 2
  Mesh *leg2 = make_cube_mesh(name + "_leg2", color, size);
  // Depends on previous leg
  leg2->dependency_index = 2;
  leg2->position = glm::vec3(0.f, 0.f, 8 * leg_scale - (leg_scale / 2));
  // Leg 3
  Mesh *leg3 = make_cube_mesh(name + "_leg3", color, size);
  leg3->dependency_index = 3;
  leg3->position = glm::vec3(body_scale * 4, 0.f, 0.f);
  // Leg 4
  Mesh *leg4 = make_cube_mesh(name + "_leg4", color, size);
  leg4->dependency_index = 2;
  leg4->position = glm::vec3(body_scale * 4, 0.f, 0.f);

  // Eyes
  // Left black
  float eye_scale = head_scale / 8.f;
  Mesh *eye_lb = make_cube_mesh(name + "_eye_lb", glm::vec3(0.f, 0.f, 0.f), size);
  // Head dependency
  eye_lb->dependency_index = 1;
  eye_lb->scale = glm::vec3(eye_scale);
  eye_lb->position = glm::vec3(
      head_scale + (eye_scale * 2),
      0.f,
      head_scale);

  Mesh *eye_lw = make_cube_mesh(name + "_eye_lw", glm::vec3(1.f, 1.f, 1.f), size);
  // eye_lb depenency
  eye_lw->dependency_index = 6;
  eye_lw->position = glm::vec3(
      0.f,
      0.f,
      -head_scale * 2.5);

  // Eyes
  // Right black
  Mesh *eye_rb = make_cube_mesh(name + "_eye_rb", glm::vec3(0.f, 0.f, 0.f), size);
  // Head dependency
  eye_rb->dependency_index = 1;
  eye_rb->scale = glm::vec3(eye_scale);
  eye_rb->position = glm::vec3(
      head_scale + (eye_scale * 2),
      0.f,
      -head_scale);

  Mesh *eye_rw = make_cube_mesh(name + "_eye_rw", glm::vec3(1.f, 1.f, 1.f), size);
  // eye_lb depenency
  eye_rw->dependency_index = 8;
  eye_rw->position = glm::vec3(
      0.f,
      0.f,
      +head_scale * 2.5);

  object->add_mesh(body);
  object->add_mesh(head);
  object->add_mesh(leg1);
  object->add_mesh(leg2);
  object->add_mesh(leg3);
  object->add_mesh(leg4);
  object->add_mesh(eye_lb);
  object->add_mesh(eye_lw);
  object->add_mesh(eye_rb);
  object->add_mesh(eye_rw);
  return std::unique_ptr<Object>(object);
}

void PigObject::update(int delta)
{
  float jump_modifier = 0.01f * static_cast<float>(delta);
  float walk_modifier = 0.04f * static_cast<float>(delta);
  float rotation_modifier = 0.08f * static_cast<float>(delta);
  float change = this->position == START ? 1.f : -1.f;

  glm::vec3 pos = this->mesh[0]->position;
  glm::vec3 rot = this->mesh[0]->rotation;

  switch (this->state)
  {
  case JUMP:
    pos += glm::vec3(0.f, jump_modifier, 0.f);
    if (pos.y >= this->y_end)
    {
      this->state = DOWN;
    }
    break;
  case DOWN:
    pos += glm::vec3(0.f, -jump_modifier, 0.f);
    if (pos.y <= this->start_pos.y)
    {
      this->state = ROTATE;
    }
    break;
  case ROTATE:
  {
    float end = 90.f * change;
    rot += glm::vec3(0.f, rotation_modifier * change, 0.f);
    if ((change == 1.f && rot.y >= end) || (change == -1.f && rot.y <= end))
    {
      this->state = MOVE;
    }
    break;
  }
  case MOVE:
  {
    pos += glm::vec3(0.f, 0.f, walk_modifier * change * -1.f);
    bool reach_end = change == 1.f && pos.z <= this->z_end;
    bool reach_start = change == -1.f && pos.z >= this->start_pos.z;
    if (reach_end || reach_start)
    {
      this->position = reach_end ? END : START;
      this->state = JUMP;
    }
    break;
  }
  }

  // if (this->state == JUMP_START)
  // {
  //   pos += glm::vec3(0.f, jump_modifier, 0.f);
  //   if (pos.y >= this->y_end)
  //   {
  //     this->state = DOWN_START;
  //   }
  // }
  // else if (this->state == DOWN_START)
  // {

  // }
  // else if (this->state == MOVE_END)
  // {
  //   pos -= glm::vec3(0.f, 0.f, walk_modifier);
  //   if (pos.z <= this->z_end)
  //   {
  //     this->state = JUMP_END;
  //   }
  // }
  // else if (this->state == JUMP_END)
  // {
  //   pos += glm::vec3(0.f, jump_modifier, 0.f);
  //   if (pos.y >= this->y_end)
  //   {
  //     this->state = DOWN_END;
  //   }
  // }
  // else if (this->state == DOWN_END)
  // {
  //   pos -= glm::vec3(0.f, jump_modifier, 0.f);
  //   if (pos.y <= this->start_pos.y)
  //   {
  //     this->state = ROTATE_END;
  //   }
  // }
  // else if (this->state == ROTATE_END)
  // {
  //   rot += glm::vec3(0.f, rotation_modifier, 0.f);
  //   if (rot.y >= 270.f)
  //   {
  //     this->state = MOVE_START;
  //   }
  // }
  // else if (this->state == MOVE_START)
  // {
  //   rot += glm::vec3(0.f, rotation_modifier, 0.f);
  //   if (rot.y >= 270.f)
  //   {
  //     this->state = MOVE_START;
  //   }
  // }

  this->mesh[0]->position = pos;
  this->mesh[0]->rotation = rot;
  update_matrices(false);
}