#include "MyFirstScene.h"

/* report GL errors, if any, to stderr */
void checkError(const char *functionName) {
  GLenum error;
  while ((error = glGetError()) != GL_NO_ERROR) {
    fprintf(stderr, "GL_ERROR:[%s]: 0x%X\n", functionName, error);
  }
}

void draw_p(Point p) { glVertex3f(p.x, p.y, p.z); }

void draw_pts(Point points[], int indices[], int size) {
  int i = 0;
  while (i < size) {
    draw_p(points[indices[i]]);
    i += 1;
  }
}

void draw_wall(float size) {
  float z = 0.f;
  Point points[] = {
      Point(-size, size, z),
      Point(-size, -size, z),
      Point(size, -size, z),
      Point(size, size, z),
  };
  int indices[] = {0, 1, 2, 0, 2, 3};
  float c = 0.5f;
  glColor3f(c, c, c);
  glBegin(GL_TRIANGLES);
  draw_pts(points, indices, 6);
  glEnd();
}

void draw_light(float size, float x_offset) {
  float z = 0.f;
  Point points[] = {
      Point(-size, size, z),
      Point(-size, -size, z),
      Point(size, -size, z),
      Point(size, size, z),
  };
  int indices[] = {0, 1, 2, 0, 2, 3};
  float c = 0.f;

  glPushMatrix();

  // Add x transform
  glTranslatef(x_offset, 0.f, 0.f);
  glBegin(GL_TRIANGLES);

  glColor3f(c, c, c);
  draw_pts(points, indices, 6);

  glEnd();
  glPopMatrix();
}

void draw() {

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear buffers
  glMatrixMode(GL_MODELVIEW); // set for model and viewing operations
  glLoadIdentity();
  glClearColor(0.f, 0.f, 0.f, 1.f); // set background colour
  glTranslatef(0.f, 0.f, -100.f);   // reset drawing
  // Rotate when user changes rotate_x and rotate_y

  glRotatef(rotate_x, 1.0, 0.0, 0.0);
  glRotatef(rotate_y, 0.0, 1.0, 0.0);
  // move drawing further back in the scene

  // Drawings
  draw_wall(20.f);
  checkError("DRAW:WALL");
  float ls = 2.f;
  float lo = 8;
  draw_light(ls, -lo);
  draw_light(ls, lo);
  checkError("DRAW:LIGHT");

  if (MojaveWorkAround) {
    glutReshapeWindow(601, 401);
    MojaveWorkAround = false;
  }
  glutPostRedisplay();
  checkError("DRAW");
  glutSwapBuffers(); // execute all commands, swap buffers
}

void setup() {
  width = 600;  // initialise global window variables
  height = 400; // define in your header: int width, height;
  glutInitDisplayMode(GLUT_DEPTH |
                      GLUT_DOUBLE);  // enable 3D rendering and double buffering
  glutInitWindowSize(width, height); // set window size
  glutCreateWindow("My Scene");      // create and show window (named MyScene)
  draw();
}

void reshape(int _width, int _height) {
  // update global dimension variables
  width = _width;
  height = _height;
  // calculate new aspect ratio
  GLdouble aspect =
      static_cast<GLdouble>(width) / static_cast<GLdouble>(height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity(); // reset matrix
  gluPerspective(45.0, aspect, 1, 1000);
  glViewport(0, 0, width, height);
  glMatrixMode(GL_MODELVIEW); // return matrix mode to modelling and viewing
}

void specialKeys(int key, int x, int y) {

  float modifier = 2.0;
  //  Right arrow - increase rotation by 5 degree
  if (key == GLUT_KEY_RIGHT)
    rotate_y += modifier;

  //  Left arrow - decrease rotation by 5 degree
  else if (key == GLUT_KEY_LEFT)
    rotate_y -= modifier;

  else if (key == GLUT_KEY_UP)
    rotate_x += modifier;

  else if (key == GLUT_KEY_DOWN)
    rotate_x -= modifier;

  //  Request display update
  glutPostRedisplay();
}

int main(int argc, char **argv) {
  glutInit(&argc, argv); // Initialise GL environment

  setup(); // Call additional initialisation commands
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glutDisplayFunc(draw); // Register scene to render contents of draw() function
  checkError("MAIN");    // Check any OpenGL errors in initialisation
  glutReshapeFunc(reshape);
  glutSpecialFunc(specialKeys);
  glEnable(GL_DEPTH_TEST); // Enable depth testing
  glDepthMask(GL_TRUE);    // Enable depth write
  glDepthFunc(GL_LEQUAL);  // Choose the depth comparison function

  glutMainLoop(); // Begin rendering sequence
  setup();
  return 0;
}
