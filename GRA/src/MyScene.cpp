#include "MyScene.h"
#include "math.h"

int width;
int height;
double rotate_y = 0;
double rotate_x = 0;
double ratio = 100;

typedef struct Point
{
  float x;
  float y;
  float z;
} Point;

Point curved_middle_point(Point p1, Point p2)
{
  Point mp;
  mp.x = (p1.x + p2.x) * 0.5;
  mp.y = (p1.y + p2.y) * 0.5;
  mp.z = (p1.z + p2.z) * 0.5;
  float md = ratio / sqrt(mp.x * mp.x + mp.y * mp.y + mp.z * mp.z);
  mp.x *= md;
  mp.y *= md;
  mp.z *= md;
  return mp;
}

void draw_point(Point p)
{
  glVertex3f(p.x, p.y, p.z);
}

void subdivide(int level, Point p1, Point p2, Point p3)
{
  if (level == 0)
  {
    draw_point(p1);
    draw_point(p2);
    draw_point(p3);
    return;
  }
  Point lp = curved_middle_point(p1, p2);
  Point bp = curved_middle_point(p2, p3);
  Point rp = curved_middle_point(p1, p3);

  int new_level = level - 1;
  // Left
  subdivide(new_level, lp, p2, bp);
  // Middle
  subdivide(new_level, lp, bp, rp);
  // Right
  subdivide(new_level, rp, bp, p3);
  // Top
  subdivide(new_level, p1, lp, rp);
}

void draw_octahedron()
{
  float size = ratio;
  int level = 5;
  glBegin(GL_TRIANGLES);
  // Set color to green
  glColor4f(0, 1, 0, 0.2);

  Point p1, p2, p3;
  // Front up face
  p1 = {.x = 0.f, .y = size, .z = 0.f};
  p2 = {.x = 0.f, .y = 0.f, .z = size};
  p3 = {.x = size, .y = 0.f, .z = 0.f};
  subdivide(level, p1, p2, p3);

  // Left up face
  p1 = {.x = 0.f, .y = size, .z = 0.f};
  p2 = {.x = -size, .y = 0.f, .z = 0.f};
  p3 = {.x = 0.f, .y = 0.f, .z = size};
  subdivide(level, p1, p2, p3);
  // Right up Face
  p1 = {.x = 0.f, .y = size, .z = 0.f};
  p2 = {.x = size, .y = 0.f, .z = 0.f};
  p3 = {.x = 0, .y = 0.f, .z = -size};
  subdivide(level, p1, p2, p3);

  // Back up Face
  p1 = {.x = 0.f, .y = size, .z = 0.f};
  p2 = {.x = -size, .y = 0.f, .z = 0.f};
  p3 = {.x = 0.f, .y = 0.f, .z = -size};
  subdivide(level, p1, p2, p3);

  // Front down Face
  p1 = {.x = 0.f, .y = 0.f, .z = size};
  p2 = {.x = 0.f, .y = -size, .z = 0.f};
  p3 = {.x = size, .y = 0.f, .z = 0.f};
  subdivide(level, p1, p2, p3);

  // Left down Point
  p1 = {.x = 0.f, .y = 0.f, .z = size};
  p2 = {.x = -size, .y = 0.f, .z = 0.f};
  p3 = {.x = 0.f, .y = -size, .z = 0.f};
  subdivide(level, p1, p2, p3);

  // Right down Point
  p1 = {.x = size, .y = 0.f, .z = 0.f};
  p2 = {.x = 0.f, .y = 0.f, .z = -size};
  p3 = {.x = 0.f, .y = -size, .z = 0.f};
  subdivide(level, p1, p2, p3);

  // Back down Point
  p1 = {.x = 0.f, .y = 0.f, .z = -size};
  p2 = {.x = -size, .y = 0.f, .z = 0.f};
  p3 = {.x = 0.f, .y = -size, .z = 0.f};
  subdivide(level, p1, p2, p3);
  glEnd();
}

void setup()
{
  width = 600;                                   // initialise global window variables
  height = 400;                                  // define in your header: int width, height;
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE); // enable 3D rendering and double buffering
  glutInitWindowSize(width, height);             // set window size
  glutCreateWindow("My Scene");                  // create and show window (named MyScene)
  draw();
}
void draw()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear buffers
  glMatrixMode(GL_MODELVIEW);                         // set for model and viewing operations
  glLoadIdentity();                                   // reset drawing
  // Rotate when user changes rotate_x and rotate_y
  glRotatef(rotate_x, 1.0, 0.0, 0.0);
  glRotatef(rotate_y, 0.0, 1.0, 0.0);
  glClearColor(1.f, 1.f, 1.f, 1.f); // set background colour
  glTranslatef(0.f, 0.f, -300.f);   // move drawing further back in the scene

  draw_octahedron();

  checkGLError();
  glutSwapBuffers(); // execute all commands, swap buffers
}

void reshape(int _width, int _height)
{
  // update global dimension variables
  width = _width;
  height = _height;
  // calculate new aspect ratio
  GLdouble aspect = static_cast<GLdouble>(width) / static_cast<GLdouble>(height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity(); // reset matrix
  gluPerspective(45.0, aspect, 1, 1000);
  glViewport(0, 0, width, height);
  glMatrixMode(GL_MODELVIEW); // return matrix mode to modelling and viewing
}

void checkGLError()
{
  int e = 0;                   // Error count
  GLenum error = glGetError(); // Get first glError
  while (GL_NO_ERROR != error) // Loop until no errors found
  {
    e++;
    printf("GL Error %i: %s\n", e, gluErrorString(error)); // Display error string
    error = glGetError();                                  // Get next glError
  }
}

void specialKeys(int key, int x, int y)
{

  //  Right arrow - increase rotation by 5 degree
  if (key == GLUT_KEY_RIGHT)
    rotate_y += 5;

  //  Left arrow - decrease rotation by 5 degree
  else if (key == GLUT_KEY_LEFT)
    rotate_y -= 5;

  else if (key == GLUT_KEY_UP)
    rotate_x += 5;

  else if (key == GLUT_KEY_DOWN)
    rotate_x -= 5;

  //  Request display update
  glutPostRedisplay();
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv); // Initialise GL environment
  glEnable(GL_DEPTH_TEST);
  setup(); // Call additional initialisation commands
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glutDisplayFunc(draw); // Register scene to render contents of draw() function
  checkGLError();        // Check any OpenGL errors in initialisation
  glutReshapeFunc(reshape);
  glutSpecialFunc(specialKeys);
  glutMainLoop(); // Begin rendering sequence
  setup();
  return 0;
}