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
  // Window dimentions
  int w_width;
  int w_height;

  // Mouse
  int last_mouse_x;
  int last_mouse_y;
  // Detect which buttons are down
  bool is_button_down[MOUSE_STATES_N];
  bool is_first_drag_move;

  // Camera views
  Camera camera;
  // Scene
  std::unique_ptr<Scene> scene;

  // Global time
  int delta_time;
  int last_frame;

  /**
   * Recalculate the camera view
   */
  void update_viewport();

public:
  Engine(int argc, char **argv, const char *title, const int &width, const int &height);
  virtual ~Engine();
  /**
   * Remove these functionalities
   * to allow for unique pointer
   */
  Engine(const Engine &) = delete;            // non construction-copyable
  Engine &operator=(const Engine &) = delete; // non copyable
  /**
   * Set up the environmnet to prepare for drawing
   */
  void initialize();

  /**
   * Handle idle function.
   * Will:
   *   Update scene 
   *   Set new delta times
   *   Post for redraw
   */
  void idle_cb();
  /**
   * Resize camera view
   */
  void reshape_cb(int width, int height);
  /**
   * Handle keyboard presses
   */
  void handle_key_cb(unsigned char key, int x, int y);
  /**
   * Draw the scene and swap buffers to see view on screen
   */
  void draw_cb();
  /**
   * Used for debugging shaders
   */
  void message_cb(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);
  /**
   * Handle mouse button presses
   */
  void mouse_button_cb(int button, int state, int x, int y);
  void mouse_move_cb(int x, int y);
  /**
   * Used to set pre-defined camera positions, depending on the mousepad key
   */
  void change_camera_pos(unsigned char key);
};

#endif
