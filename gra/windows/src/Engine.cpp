#include "Engine.h"

Engine::Engine(
    int argc,
    char **argv,
    const char *title,
    const int &width,
    const int &height) : last_mouse_x(-1),
                         last_mouse_y(-1),
                         is_first_drag_move(true),
                         delta_time(0),
                         last_frame(0)

{
  glutInit(&argc, argv);

  this->camera = Camera();

  // Start of room
  change_camera_pos('1');

  this->w_width = width;
  this->w_height = height;
  // Because of unique ptr, need to create it manually
  this->scene = std::unique_ptr<Scene>(new Scene());
  this->scene->set_camera(&(this->camera));

  // Make sure button states are initialized
  for (int i = 0; i < MOUSE_STATES_N; i += 1)
  {
    this->is_button_down[i] = false;
  }

  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
  glutInitWindowSize(this->w_width, this->w_height);
  glutInitWindowPosition(0, 0);
  glutCreateWindow(title);

#ifndef __linux__
  if (!gladLoadGL()) {
	  printf("Something went wrong!\n");
	  exit(-1);
  }
#endif

  // Allow cube maps
  glEnable(GL_TEXTURE_CUBE_MAP);
  // GL options
  glEnable(GL_DEPTH_TEST);
  // Not drawn if not seen
  glEnable(GL_CULL_FACE);
  glEnable(GL_COLOR_MATERIAL);

  glEnable(GL_BLEND);
  // Blend colors of polygons
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  // Back will not be shown
  glCullFace(GL_BACK);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  // Set front face as shown one
  glFrontFace(GL_CCW);
  // Set to fill the whole shape
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  // glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

#ifdef GRA_DEBUG
  Log::check_error("Engine:creation");
#endif // DEBUG
}

Engine::~Engine()
{
}

void Engine::draw_cb()
{
  glClearColor(0.1f, 0.1f, 0.1f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  // DO RENDERS HERE
  this->scene->draw();

  glutSwapBuffers();
  glFlush();
#ifdef GRA_DEBUG
  Log::check_error("Engine:flush");
#endif // DEBUG
  // DO RESETS HERE
  glBindVertexArray(0);
  glUseProgram(0);

  this->scene->clear();

#ifdef GRA_DEBUG
  Log::check_error("Engine:clear");
#endif // DEBUG
}

void Engine::change_camera_pos(unsigned char key)
{
  glm::vec3 pos;
  float yaw;
  switch (key)
  {
  case '1' /* constant-expression */:
    yaw = -183.f;
    pos = glm::vec3(76.5f, 60.f, -30.6f);
    break;
  case '2':
    yaw = 0.6f;
    pos = glm::vec3(61.6f, 60.4f, -8.4f);
    break;
  case '3':
    yaw = -326.f;
    pos = glm::vec3(11.5f, 80.6f, -66.5f);
    break;
  case '4':
    yaw = -41.f;
    pos = glm::vec3(4.3f, 60.6f, -10.5f);
    break;
  default:
    return;
  }

  this->camera.yaw = yaw;
  this->camera.update_front();
  this->camera.update_position(pos);
}

void Engine::handle_key_cb(unsigned char key, int x, int y)
{
  float change = 0.05f * static_cast<float>(this->delta_time);
  glm::vec3 current_position = this->camera.position;
  if (key == 'w')
  {
    current_position += change * this->camera.front;
  }
  else if (key == 's')
  {
    current_position -= change * this->camera.front;
  }
  else if (key == 'a')
  {
    current_position -= glm::normalize(glm::cross(this->camera.front, this->camera.world_up)) * change;
  }
  else if (key == 'd')
  {
    current_position += glm::normalize(glm::cross(this->camera.front, this->camera.world_up)) * change;
  }
  else if (key == 'q')
  {
    current_position.y += change;
  }
  else if (key == 'e')
  {
    current_position.y -= change;
  }
  else if (key == '#')
  {
    std::cout << this->camera.to_string() << std::endl;
  }

  this->camera.update_position(current_position);
  change_camera_pos(key);
}

void Engine::reshape_cb(int width, int height)
{
  this->w_width = width;
  this->w_height = height;
  update_viewport();
}

void Engine::idle_cb()
{
  int frame = glutGet(GLUT_ELAPSED_TIME);
  this->delta_time = frame - this->last_frame;
  this->last_frame = frame;
  this->scene->update(this->delta_time);
  glutPostRedisplay();
}

void Engine::initialize()
{

  // glClearColor(0.f, 0.f, 0.f, 0.f);
  update_viewport();

#ifdef GRA_DEBUG
  Log::check_error("Engine:pre:initalize");
#endif // DEBUG

  // Will initialize scenes here
  this->scene->initialize();
  glutMainLoop();
}

void Engine::update_viewport()
{
  int width = this->w_width;
  int height = this->w_height;
  glViewport(0, 0, width, height);
  this->camera.update_projection(width, height);
}

void Engine::message_cb(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
  fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
          (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
          type, severity, message);
}

void Engine::mouse_button_cb(int button, int state, int x, int y)
{
  bool is_down = state == GLUT_DOWN;
  bool is_left_down = is_down && button == GLUT_LEFT_BUTTON;

  // Wheel scroll
  if (button == 3 || button == 4)
  {
    float offset = 4.f;
    float multiplier = button == 3 ? -1.f : 1.f;
    this->camera.add_to_fov(offset * multiplier, this->w_width, this->w_height);
  }

  this->is_first_drag_move = is_left_down;
  this->is_button_down[button] = is_down;
  glutPostRedisplay();
}

void Engine::mouse_move_cb(int x, int y)
{
  // Drag is happening, so we want to rotate
  if (is_button_down[GLUT_LEFT_BUTTON] && !this->is_first_drag_move)
  {
    float x_offset = (x - last_mouse_x) * this->camera.sensitivity;
    float y_offset = (y - last_mouse_y) * this->camera.sensitivity;
    this->camera.add_to_yaw(x_offset);
    this->camera.add_to_pitch(y_offset);
    this->camera.update_front();
  }
  last_mouse_x = x;
  last_mouse_y = y;
  this->is_first_drag_move = false;

  glutPostRedisplay();
}
