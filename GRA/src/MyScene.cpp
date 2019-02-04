#include "MyScene.h"

int width;
int height;

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
  glClearColor(1.f, 1.f, 1.f, 1.f);                   // set background colour
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear buffers
  glMatrixMode(GL_MODELVIEW);                         // set for model and viewing operations
  glLoadIdentity();                                   // reset drawing

  glTranslatef(0.f, 0.f, -100.f); // move drawing further back in the scene
  glColor3f(0.f, 0.f, 0.f);       // set draw colour to black
  glutWireCube(10.f);             // draw outlined cube

  checkGLError();
  glutSwapBuffers(); // execute all commands, swap buffers
}

// void draw()
// {
//   glMatrixMode(GL_MODELVIEW);
//   glLoadIdentity();
//   glTranslatef(0.f, 0.f, -100.f);
//   glClearColor(0.f, 0.f, 1.f, 1.f);
//   glClear(GL_COLOR_BUFFER_BIT);
//   glColor3f(0.f, 0.f, 0.f);
//   glutSolidCube(10);

//   checkGLError();
//   glColor3f(0.f, 0.f, 0.f);
// }

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

int main(int argc, char **argv)
{
  glutInit(&argc, argv); // Initialise GL environment
  setup();               // Call additional initialisation commands
  glutDisplayFunc(draw); // Register scene to render contents of draw() function
  checkGLError();        // Check any OpenGL errors in initialisation
  glutReshapeFunc(reshape);
  glutMainLoop(); // Begin rendering sequence
  setup();
  return 0;
}
