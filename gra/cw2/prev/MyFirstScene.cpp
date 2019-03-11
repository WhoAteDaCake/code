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
  float c = 0.0f;
  // static GLfloat ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
  // White
  // static GLfloat diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
  // No specular (black)
  static GLfloat specular[] = {1.0f, 1.0f, 1.0f, 1.0f};

  glPushAttrib(GL_ALL_ATTRIB_BITS);

  glDisable(GL_COLOR_MATERIAL);
  glEnable(GL_LIGHTING);

  // glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
  // glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
  GLfloat mat_shininess[] = {50.0};
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(c, c, c);
  glBegin(GL_TRIANGLES);
  glNormal3f(0.f, 0.f, 1.f);

  // GLfloat _element_size = 1.f / 100.f;
  // for (GLfloat i = -size; i < size; i += _element_size)
  // {
  // 	for (GLfloat j = -size; j < size; j += _element_size)
  // 	{
  // 		glBegin(GL_TRIANGLES);
  // 		{
  // 			glNormal3f(0.0f, 0.0f, 1.0f);

  // 			glVertex3f(i, j, 0.0f);
  // 			glVertex3f(i + _element_size, j, 0.0f);
  // 			glVertex3f(i, j + _element_size, 0.0f);

  // 			glVertex3f(i + _element_size, j, 0.0f);
  // 			glVertex3f(i + _element_size, j + _element_size, 0.0f);
  // 			glVertex3f(i, j + _element_size, 0.0f);
  // 		}
  // 		glEnd();
  // 	}
  // }
  draw_pts(points, indices, 6);

  glEnd();
  glPopAttrib();
}

void draw_light(float size, float x_offset, GLenum lightSrc, GLfloat *ambient,
                GLfloat *diffuse, GLfloat *specular) {
  float z = size + size / 2;

  glEnable(GL_LIGHTING);
  // Colour
  glLightfv(lightSrc, GL_AMBIENT, ambient);
  glLightfv(lightSrc, GL_DIFFUSE, diffuse);
  glLightfv(lightSrc, GL_SPECULAR, specular);

  // Position
  GLfloat pos[] = {0.f + x_offset, 0.f, z, -1.f};
  glLightfv(lightSrc, GL_POSITION, pos);

  // Fall off

  glLightf(lightSrc, GL_CONSTANT_ATTENUATION, 0.f);
  glLightf(lightSrc, GL_LINEAR_ATTENUATION, 0.f);
  glLightf(lightSrc, GL_QUADRATIC_ATTENUATION, 0.f);

  glEnable(lightSrc);
}

void draw() {

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear buffers
  glMatrixMode(GL_MODELVIEW); // set for model and viewing operations
  glLoadIdentity();
  glClearColor(0.f, 0.f, 0.f, 1.f); // set background colour
  glTranslatef(0.f, 0.f, -50.f);    // reset drawing
  // Rotate when user changes rotate_x and rotate_y

  glRotatef(rotate_x, 1.0, 0.0, 0.0);
  glRotatef(rotate_y, 0.0, 1.0, 0.0);
  // move drawing further back in the scene

  // Drawings
  draw_wall(20.f);
  checkError("DRAW:WALL");
  float ls = 20.f;
  float lo = 8;
  static GLfloat ambient_A[] = {0.1f, 0.0f, 0.0f, 1.0f};
  static GLfloat diffuse_A[] = {0.9f, 0.0f, 0.0f, 1.0f};
  static GLfloat specular_A[] = {0.0f, 0.0f, 0.0f, 1.0f};
  draw_light(ls, -lo, GL_LIGHT0, ambient_A, diffuse_A, specular_A);
  // draw_light(ls, lo);
  checkError("DRAW:LIGHT");

  if (MojaveWorkAround) {
    glutReshapeWindow(601, 401);
    MojaveWorkAround = false;
  }
  glutPostRedisplay();
  checkError("DRAW");
  glFlush();
  glutSwapBuffers(); // execute all commands, swap buffers
}

void setup() {
  width = 600;  // initialise global window variables
  height = 400; // define in your header: int width, height;

  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(width, height); // set window size
  glutCreateWindow("My Scene");      // create and show window (named MyScene)

  // Light
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

  // set the ambient light model
  GLfloat global_ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

  // Enable smooth shading from lighting
  glShadeModel(GL_SMOOTH);

  // Enable automatic normalisation of normal vectors
  glEnable(GL_NORMALIZE);
  // Enable depth testing

  // Enable backface culling
  glEnable(GL_CULL_FACE);

  // Enable transparency blending
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_DEPTH_TEST); // Enable depth testing
  glDepthMask(GL_TRUE);    // Enable depth write
  glDepthFunc(GL_LEQUAL);  // Choose the depth comparison function

  // Set lighting intensity and color
  // GLfloat qaAmbientLight[] = {0.2, 0.2, 0.2, 1.0};
  // GLfloat qaDiffuseLight[] = {0.8, 0.8, 0.8, 1.0};
  // GLfloat qaSpecularLight[] = {1.0, 1.0, 1.0, 1.0};
  // glLightfv(GL_LIGHT0, GL_AMBIENT, qaAmbientLight);
  // glLightfv(GL_LIGHT0, GL_DIFFUSE, qaDiffuseLight);
  // glLightfv(GL_LIGHT0, GL_SPECULAR, qaSpecularLight);

  // // Set the light position
  // GLfloat qaLightPosition[] = {.5, .5, 0.0, 1.0};
  // glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition);

  // draw();
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
  glutMainLoop(); // Begin rendering sequence
  setup();
  return 0;
}
