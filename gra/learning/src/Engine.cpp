#include "Engine.h"

Engine *Engine::activeEngine = NULL;

void Engine::idle()
{
  Engine::activeEngine->idle_cb();
}

void Engine::reshape(int width, int height)
{
  Engine::activeEngine->reshape_cb(width, height);
}

void Engine::handle_key(unsigned char key, int x, int y)
{
  Engine::activeEngine->handle_key_cb(key, x, y);
}

void Engine::draw()
{
  Engine::activeEngine->draw_cb();
}

Engine::Engine(int argc, char **argv, const char *title, const int &width, const int &height)
{
  glutInit(&argc, argv);

  this->w_width = width;
  this->w_height = height;
  this->camera = Camera();

  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
  glutInitWindowSize(this->w_width, this->w_height);
  glutInitWindowPosition(200, 200);
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

  Engine::activeEngine = this;
}

Engine::~Engine()
{
}

void Engine::draw_cb()
{
  glClearColor(0.f, 0.f, 0.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  // DO RENDERS HERE

  glutSwapBuffers();
  glFlush();
  Log::check_error("display");
  // DO RESETS HERE
  glBindVertexArray(0);
  glUseProgram(0);

  Log::check_error("reset");
}

void Engine::handle_key_cb(unsigned char key, int x, int y)
{
  // TODO
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

  glClearColor(0.f, 0.f, 0.f, 0.f);
  update_viewport();

  Log::check_error("Initialize");
  // Will initialize scenes here

  glutDisplayFunc(Engine::draw);
  glutKeyboardFunc(Engine::handle_key);
  glutReshapeFunc(Engine::reshape);
  glutIdleFunc(Engine::idle);
  glutMainLoop();
}

void Engine::update_viewport()
{
  int width = this->w_width;
  int height = this->w_height;
  glViewport(0, 0, width, height);
  this->camera.update_projection(width, height);
}