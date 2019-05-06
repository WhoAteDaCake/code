#pragma once

#ifndef ENGINE_H
#define ENGINE_H

#include "Log.h"
#include "Scene.h"

class Engine
{
private:
  int w_width;
  int w_height;
  Camera camera;
  Scene scene;

  void update_viewport();

public:
  Engine(int argc, char **argv, const char *title, const int &width, const int &height);
  virtual ~Engine();

  void initialize();

  // GLUT specific handlers
  // Because glut uses callbacks, it's more straight forward
  // To use static function callbacks
  static Engine *activeEngine;
  static void idle();
  static void reshape(int width, int height);
  static void handle_key(unsigned char key, int x, int y);
  static void draw();
  static void message(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);

  void idle_cb();
  void reshape_cb(int width, int height);
  void handle_key_cb(unsigned char key, int x, int y);
  void draw_cb();
  void message_cb(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);
};

#endif
