#include "MyScene.h"

void setup()
{
}

void draw()
{
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
  glutMainLoop();        // Begin rendering sequence
  return 0;
}
