#ifndef ___MyFirstScene__
#define ___MyFirstScene__

#include <iostream>
#include <math.h>
#include <stdio.h>
#include <string>

#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

struct Point {
  float x;
  float y;
  float z;
  Point(float x, float y, float z) : x(x), y(y), z(z) {}
  Point() : x(0.f), y(0.f), z(0.f) {}
  std::string to_string() {
    char buff[50];
    snprintf(buff, sizeof(buff), "(%f, %f, %f)", x, y, z);
    std::string result = buff;
    return result;
  }
};

// ----------------------------------------------------------
// Function Prototypes
// ----------------------------------------------------------
void setup();
void draw();
Point curved_middle_point(Point p1, Point p2);
void draw_point(Point p);
void subdivide(int level, Point p1, Point p2, Point p3);
void draw_octahedron();
void reshape(int _width, int _height);
void checkGLError(); // Prints any OpenGL errors to console
void specialKeys(int key, int x, int y);
int main(int argc, char **argv);

// ----------------------------------------------------------
// Global Variables
// ----------------------------------------------------------
double rotate_y = 0;
double rotate_x = 0;
int width = 0;
int height = 0;
float radius = 100;

#ifdef __APPLE_
bool MojaveWorkAround = true;
#else
bool MojaveWorkAround = false;
#endif

#endif