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
  this->camera.yaw = 91.f;
  // this->camera.pitch = -79.6f;
  this->camera.update_front();
  this->camera.update_position(glm::vec3(2.7f, 0.02, -7.2f));

  // Start of scene
  // this->camera.yaw = -180.f;
  // this->camera.update_front();
  // this->camera.update_position(glm::vec3(26.f, 26.5f, -27.4f));

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

#ifdef GRA_DEBUG
  Log::check_error("Engine:creation");
#endif // DEBUG
}

Engine::~Engine()
{
}

void Engine::draw_cb()
{
  int frame = glutGet(GLUT_ELAPSED_TIME);
  this->delta_time = frame - this->last_frame;
  this->last_frame = frame;
  glClearColor(0.f, 0.f, 0.f, 0.f);
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
}

void Engine::reshape_cb(int width, int height)
{
  this->w_width = width;
  this->w_height = height;
  update_viewport();
}

void Engine::idle_cb()
{
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