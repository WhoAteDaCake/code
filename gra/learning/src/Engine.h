#pragma once

#ifndef ENGINE_H
#define ENGINE_H

#define MOUSE_STATES_N 5

#include "Log.h"
#include "Scene.h"
#include <memory>

class Engine
{
private:
  int w_width;
  int w_height;

  // Mouse
  int last_mouse_x;
  int last_mouse_y;
  bool is_button_down[MOUSE_STATES_N];
  bool is_first_drag_move;

  // Camera views
  Camera camera;
  std::unique_ptr<Scene> scene;

  void update_viewport();

public:
  Engine(int argc, char **argv, const char *title, const int &width, const int &height);
  virtual ~Engine();
  Engine(const Engine &) = delete;            // non construction-copyable
  Engine &operator=(const Engine &) = delete; // non copyable

  void initialize();

  void idle_cb();
  void reshape_cb(int width, int height);
  void handle_key_cb(unsigned char key, int x, int y);
  void draw_cb();
  void message_cb(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);
  void mouse_button_cb(int button, int state, int x, int y);
  void mouse_move_cb(int x, int y);
};

#endif
