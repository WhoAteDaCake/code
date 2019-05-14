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
  mesh->auto_normals = true;
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
      // faces[k].normal = normals[normal_i];
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
  std::shared_ptr<Material> material = std::make_shared<Material>(
      glm::vec3(0.2f),
      glm::vec3(0.4f),
      glm::vec3(0.2f), true);
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

std::unique_ptr<ObjectLight> make_light_cube(
    std::string name,
    std::unique_ptr<MaterialManager> &mat_manager,
    glm::vec3 color,
    glm::vec3 position,
    glm::vec3 rotation,
    glm::vec3 scale)
{
  std::shared_ptr<Material> material = std::make_shared<Material>(glm::vec3(0.3f), glm::vec3(0.3f), glm::vec3(0.3f), true);
  material->toggle_ignore_light(true);

  Mesh *cube = make_cube_mesh(name + "_light_cube", color, 1.f);
  cube->position = position;
  cube->rotation = rotation;
  cube->scale = scale;

  Cylinder *handle = new Cylinder(name + "_handle", glm::vec3(0.f));
  handle->dependency_index = 0;
  handle->scale = glm::vec3(0.5f, 6.f, 0.5f);
  handle->position = glm::vec3(0.f, 1.f, 0.f);

  Object *object = new Object(name, nullptr, nullptr, material, cube);
  object->add_mesh(handle);

  Light *light = new Light(position);
  ObjectLight *object_light = new ObjectLight(
      std::unique_ptr<Object>(object),
      std::unique_ptr<Light>(light));
  return std::unique_ptr<ObjectLight>(object_light);
}

std::unique_ptr<ObjectLight> make_fan_lamp(
    std::string name,
    std::unique_ptr<MaterialManager> &mat_manager,
    glm::vec3 color,
    glm::vec3 position,
    glm::vec3 rotation,
    glm::vec3 scale)
{
  std::shared_ptr<Material> material = std::make_shared<Material>(
      glm::vec3(0.8f),
      glm::vec3(1.f),
      glm::vec3(0.4f), true);
  // material->toggle_ignore_light(true);

  float ch_ratio = 0.4f;
  // Cylinders
  Cylinder *middle_c = new Cylinder(name + "_middle_c", glm::vec3(0.f));
  middle_c->auto_normals = true;
  middle_c->position = position;
  middle_c->rotation = rotation;
  middle_c->scale = scale * glm::vec3(1.f, ch_ratio, 1.f);

  Cylinder *down_c = new Cylinder(name + "_down_c", glm::vec3(0.75f));
  down_c->auto_normals = true;
  down_c->dependency_index = 0;
  down_c->auto_normals = true;
  down_c->position = glm::vec3(0.f, -1.f, 0.f);

  Cylinder *up_c = new Cylinder(name + "_up_c", glm::vec3(0.75f));
  up_c->auto_normals = true;
  up_c->dependency_index = 0;
  up_c->auto_normals = true;
  up_c->position = glm::vec3(0.f, 1.f, 0.f);

  // Bottom lamp
  HalfSphere *sphere = new HalfSphere(name + "_middle_c", color, 5);
  sphere->auto_normals = true;
  sphere->dependency_index = 1;
  sphere->scale = glm::vec3(1.f, 1.f / ch_ratio, 1.f) * glm::vec3(0.8f);
  // Fan blades
  float fan_ratio = ch_ratio / 4.f;
  Mesh *blade1 = make_cube_mesh(name + "_blade_1", glm::vec3(0.5f, 0.27f, 0.07f), 1.f);
  blade1->auto_normals = true;
  blade1->dependency_index = 0;
  blade1->scale = glm::vec3(ch_ratio, fan_ratio, 1.5f);
  blade1->position = glm::vec3(0.f, ch_ratio, -ch_ratio * 3.6f);
  blade1->rotation = glm::vec3(0.f, 0.f, 30.f);

  Mesh *blade2 = make_cube_mesh(name + "_blade_2", glm::vec3(0.5f, 0.27f, 0.07f), 1.f);
  blade2->auto_normals = true;
  blade2->dependency_index = 4;
  blade2->position = glm::vec3(0.f, 0.f, blade1->position.z * -1.4f);
  blade2->rotation = glm::vec3(0.f, 0.f, 180.f);

  FanObject *object = new FanObject(name, nullptr, nullptr, material);
  object->add_mesh(middle_c);
  object->add_mesh(down_c);
  object->add_mesh(up_c);
  object->add_mesh(sphere);
  object->add_mesh(blade1);
  object->add_mesh(blade2);

  // Want to make sure the light is at spheres location
  Light *light = new Light(position);
  ObjectLight *object_light = new ObjectLight(
      std::unique_ptr<Object>(object),
      std::unique_ptr<Light>(light));
  return std::unique_ptr<ObjectLight>(object_light);
}

void PigObject::initialize()
{
  // Position of body is initial position
  this->start_pos = this->mesh[0]->position;
  this->z_end = this->start_pos.z - this->walk;
  this->y_end = this->start_pos.y + this->jump;
  this->y_velocity = this->y_velocity_max;
  Object::initialize();
}

void PigObject::update(int delta)
{
  float jump_modifier = 0.02f * static_cast<float>(delta);
  float walk_modifier = 0.02f * static_cast<float>(delta);
  float rotation_modifier = 0.15f * static_cast<float>(delta);
  float change = this->position == START ? 1.f : -1.f;
  float leg_rot_max = 12.f;

  glm::vec3 pos = this->mesh[0]->position;
  glm::vec3 rot = this->mesh[0]->rotation;
  // Just need to modify 1 leg, as all of the others depend on it
  glm::vec3 leg_rot = this->mesh[2]->rotation;

  switch (this->state)
  {
  case JUMP:
  {
    pos += glm::vec3(0.f, jump_modifier + y_velocity, 0.f);
    this->y_velocity = this->y_velocity * 1.18 - jump_modifier / 2;
    if (this->y_velocity <= 0)
    {
      this->y_velocity = 0;
    }
    if (pos.y >= this->y_end)
    {
      this->state = DOWN;
      this->y_velocity = 0.f;
    }
    break;
  }
  case DOWN:
    pos += glm::vec3(0.f, -jump_modifier - y_velocity, 0.f);
    this->y_velocity = this->y_velocity * 0.82 + jump_modifier / 2;
    if (this->y_velocity > this->y_velocity_max)
    {
      this->y_velocity = this->y_velocity_max;
    }
    if (pos.y <= this->start_pos.y)
    {
      this->state = ROTATE;
      this->y_velocity = this->y_velocity_max;
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
    // TODO feet rotation
    pos += glm::vec3(0.f, 0.f, walk_modifier * change * -1.f);
    bool reach_end = change == 1.f && pos.z <= this->z_end;
    bool reach_start = change == -1.f && pos.z >= this->start_pos.z;
    // Leg rotation
    float leg_change = this->leg_position == START ? 1.f : -1.f;
    float change = leg_change * abs(this->walk) / (leg_rot_max * 3.f);
    leg_rot += glm::vec3(0.f, 0.f, change);
    bool leg_end = leg_change == 1.f && leg_rot.z >= leg_rot_max;
    bool leg_start = leg_change == -1.f && leg_rot.z <= -leg_rot_max;
    if (leg_end || leg_start)
    {
      this->leg_position = leg_end ? END : START;
    }

    if (reach_end || reach_start)
    {
      this->position = reach_end ? END : START;
      this->state = JUMP;
    }
    break;
  }
  }

  this->mesh[0]->position = pos;
  this->mesh[0]->rotation = rot;
  this->mesh[2]->rotation = leg_rot;
  update_matrices(false);
}

void FanObject::update(int delta)
{
  float rotation_modifier = 0.5f * static_cast<float>(delta);
  float new_angle = angle + rotation_modifier;

  if (new_angle > 360.f)
  {
    new_angle -= 360.f;
  }

  this->mesh[0]->rotation.y = new_angle;

  this->angle = new_angle;
  update_matrices(false);
}