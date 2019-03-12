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

double rotate_y = 0;
double rotate_x = 0;
int window_w = 600;
int window_h = 400;
const char *window_title = "Application";

/* report GL errors, if any, to stderr */
void checkError(const char *functionName)
{
  GLenum error;
  while ((error = glGetError()) != GL_NO_ERROR)
  {
    fprintf(stderr, "GL_ERROR:[%s]: 0x%X\n", functionName, error);
  }
}

void resize(int w, int h)
{
  window_w = w;
  window_h = h;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(static_cast<GLdouble>(-window_w) / static_cast<GLdouble>(2.0), static_cast<GLdouble>(window_w) / static_cast<GLdouble>(2.0),
            static_cast<GLdouble>(-window_h) / static_cast<GLdouble>(2.0), static_cast<GLdouble>(window_h) / static_cast<GLdouble>(2.0),
            static_cast<GLdouble>(1.0), static_cast<GLdouble>(1000.0));
  // Reset back to 3D
  GLdouble aspect = window_w / window_h;
  gluPerspective(60.0, aspect, 1.0, 1000.0);
  glMatrixMode(GL_MODELVIEW);
  checkError("RESIZE");
}

void initialize()
{
  // Default background colour
  glClearColor(static_cast<GLclampf>(0.0f), static_cast<GLclampf>(0.0f),
               static_cast<GLclampf>(0.0f), static_cast<GLclampf>(1.0f));

  // Enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  // Enable backface culling
  glEnable(GL_CULL_FACE);

  // Enable transparency blending
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Enable 2-D texturing
  glEnable(GL_TEXTURE_2D);

  // Enable blending colour information with texture information
  glEnable(GL_COLOR_MATERIAL);

  // Turn off 2 sided lighting
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

  // set the ambient light model
  GLfloat global_ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

  // enable directional light lighting
  // (x, y, z, 0.0) -> directional lighting
  // (x, y, z, 1.0) -> positional lighting
  glEnable(GL_LIGHTING);
  GLfloat ambience[] = {0.2f, 0.2f, 0.2f, 1.0f};
  GLfloat diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
  GLfloat specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
  GLfloat position[] = {1.0f, 1.0f, 1.0f, 0.0f};
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambience);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
  glLightfv(GL_LIGHT0, GL_POSITION, position);
  glEnable(GL_LIGHT0);

  // Enable smooth shading from lighting
  glShadeModel(GL_SMOOTH);

  // Enable automatic normalisation of normal vectors
  glEnable(GL_NORMALIZE);
  glViewport(0.f, 0.f, window_w, window_h);

  // float m = 0.5f * static_cast<float>(window_h) / static_cast<float>(tan(M_PI / 6.0));
  // gluLookAt(0.f, 0.f, m, 0.f, 0.f, m - 1.0f, 0.f, 1.f, 0.f);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  checkError("INITIALIZE");
}

void draw_wall(GLfloat pos[], GLfloat scale[])
{
  static GLfloat ambient[] = {0.5f, 0.5f, 0.5f, 1.0f};
  // White
  static GLfloat diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
  // No specular (black)
  static GLfloat specular[] = {0.0f, 0.0f, 0.0f, 1.0f};
  glPushMatrix();
  glPushAttrib(GL_ALL_ATTRIB_BITS);

  glTranslatef(pos[0], pos[1], pos[2]);
  // glScalef(scale[0], scale[1], scale[2]);

  glDisable(GL_COLOR_MATERIAL);
  glEnable(GL_LIGHTING);
  // glColor3f(1.0f, 0.f, 0.f);

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 1.0f);

  glutSolidCube(200);
  // GLfloat _element_size = 1.0f / 100;

  // for (GLfloat i = -0.5f; i < 0.5f; i += _element_size)
  // {
  //   for (GLfloat j = -0.5f; j < 0.5f; j += _element_size)
  //   {
  //     glBegin(GL_TRIANGLES);
  //     {
  //       glNormal3f(0.0f, 0.0f, 1.0f);

  //       glVertex3f(i, j, 0.0f);
  //       glVertex3f(i + _element_size, j, 0.0f);
  //       glVertex3f(i, j + _element_size, 0.0f);

  //       glVertex3f(i + _element_size, j, 0.0f);
  //       glVertex3f(i + _element_size, j + _element_size, 0.0f);
  //       glVertex3f(i, j + _element_size, 0.0f);
  //     }
  //     glEnd();
  //   }
  // }

  glPopAttrib();
  glPopMatrix();
  checkError("DRAW:WALL");
}

void draw_light(GLenum lightSrc, GLfloat *ambient, GLfloat *diffuse, GLfloat *specular, GLfloat *position)
{
  glEnable(GL_LIGHTING);
  // Colour
  glLightfv(lightSrc, GL_AMBIENT, ambient);
  glLightfv(lightSrc, GL_DIFFUSE, diffuse);
  glLightfv(lightSrc, GL_SPECULAR, specular);

  // Position
  glLightfv(lightSrc, GL_POSITION, position);

  // Fall off
  glLightf(lightSrc, GL_CONSTANT_ATTENUATION, 0.f);
  glLightf(lightSrc, GL_LINEAR_ATTENUATION, 0.025f);
  glLightf(lightSrc, GL_QUADRATIC_ATTENUATION, 0.00005f);

  glEnable(lightSrc);
}

void draw()
{
  // Clear colour and depth buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  resize(window_w, window_h);
  // Reset MODELVIEW matrix
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glPushMatrix();
  glTranslatef(0.f, 0.f, 10.f);

  glRotatef(rotate_x, 1.0, 0.0, 0.0);
  glRotatef(rotate_y, 0.0, 1.0, 0.0);

  GLfloat wall_pos[] = {0.f, 0.f, -300.f};
  GLfloat wall_scale[] = {800.f, 600.f, 1.0f};
  draw_wall(wall_pos, wall_scale);

  static GLfloat ambient_A[] = {0.5f, 0.0f, 0.0f, 1.0f};
  static GLfloat diffuse_A[] = {0.9f, 0.0f, 0.0f, 1.0f};
  static GLfloat specular_A[] = {0.0f, 0.0f, 0.0f, 1.0f};
  static GLfloat position_A[] = {-100.0f, 0.0f, 400.0f, 1.0f};
  draw_light(GL_LIGHT0, ambient_A, diffuse_A, specular_A, position_A);

  static GLfloat ambient_B[] = {0.0f, 0.0f, 0.5f, 1.0f};
  static GLfloat diffuse_B[] = {0.0f, 0.0f, 0.9f, 1.0f};
  static GLfloat specular_B[] = {0.0f, 0.0f, 0.0f, 1.0f};
  static GLfloat position_B[] = {100.0f, 0.0f, 400.0f, 1.0f};

  draw_light(GL_LIGHT1, ambient_B, diffuse_B, specular_B, position_B);
  glPopMatrix();
  // Zealous reset of MODELVIEW matrix
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glutSwapBuffers();
  checkError("DRAW");
}

void special_keys(int key, int x, int y)
{

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

int main(int argc, char **argv)
{
  glutInit(&argc, argv);

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(window_w, window_h);
  glutInitWindowPosition(100, 100);
  glutCreateWindow(window_title);

  resize(window_w, window_h);
  glutDisplayFunc(draw);
  glutReshapeFunc(resize);
  glutSpecialFunc(special_keys);

  initialize();

  printf("WINDOW:[%dx%d]\n", window_w, window_h);
  printf("OpenGL:[%s]\n", glGetString(GL_VERSION));

  glutMainLoop();

  return 0;
}