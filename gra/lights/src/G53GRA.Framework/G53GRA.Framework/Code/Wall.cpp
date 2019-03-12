#include "Wall.h"

#define _TESSELATION_RES 100.0f;

GLfloat mp(GLfloat p1[], GLfloat p2[], int i)
{
  return (p1[i] + p2[i]) * 0.5f;
}

GLfloat *middle_vertex(GLfloat p1[], GLfloat p2[])
{
  GLfloat *p = (GLfloat *)malloc(sizeof(GLfloat) * 3);
  p[0] = mp(p1, p2, 0);
  p[1] = mp(p1, p2, 1);
  p2[2] = mp(p1, p2, 2);
  return p;
}

void subdivide(int level, GLfloat p1[], GLfloat p2[], GLfloat p3[])
{
  if (level == 0)
  {
    glBegin(GL_TRIANGLES);
    glNormal3f(0.f, 0.f, 1.0f);
    glVertex3fv(p1);
    glVertex3fv(p2);
    glVertex3fv(p3);
    glEnd();
    return;
  }
  GLfloat *lp = middle_vertex(p1, p2);
  GLfloat *bp = middle_vertex(p2, p3);
  GLfloat *rp = middle_vertex(p1, p3);

  int new_level = level - 1;
  // Left
  subdivide(new_level, lp, p2, bp);
  // Middle
  subdivide(new_level, lp, bp, rp);
  // Right
  subdivide(new_level, rp, bp, p3);
  // Top
  subdivide(new_level, p1, lp, rp);
  free(lp);
  free(bp);
  free(rp);
}

Wall::Wall()
{
  // Grey
  static GLfloat mat_ambient[] =
      {
          0.5f, 0.5f, 0.5f, 1.0f};
  _mat_ambient = mat_ambient;
  // White
  static GLfloat mat_diffuse[] =
      {
          1.0f, 1.0f, 1.0f, 1.0f};
  _mat_diffuse = mat_diffuse;
  // No specular (black)
  static GLfloat mat_specular[] =
      {
          0.0f, 0.0f, 0.0f, 1.0f};
  _mat_specular = mat_specular;
  // Matte (not shiny)
  _mat_shininess = 1.0f;
}

void Wall::Display()
{
  glPushMatrix();
  glPushAttrib(GL_ALL_ATTRIB_BITS);

  glTranslatef(pos[0], pos[1], pos[2]);
  glScalef(scale[0], scale[1], scale[2]);

  glDisable(GL_COLOR_MATERIAL);
  glEnable(GL_LIGHTING);

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, _mat_ambient);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, _mat_diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, _mat_specular);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, _mat_shininess);

  float size = 0.5f;
  float z = 0.f;

  GLfloat p[4][3] = {
      {-size, size, z},
      {-size, -size, z},
      {size, -size, z},
      {size, size, z}};

  int level = 5;
  subdivide(level, p[0], p[1], p[2]);
  subdivide(level, p[0], p[2], p[3]);
  // glBegin(GL_TRIANGLES);
  // glNormal3f(0.f, 0.f, 1.0f);

  // glVertex3fv(points[0]);
  // glVertex3fv(points[1]);
  // glVertex3fv(points[2]);

  // glVertex3fv(points[0]);
  // glVertex3fv(points[2]);
  // glVertex3fv(points[3]);

  // glEnd();

  // GLfloat _element_size = 1.0f / _TESSELATION_RES;

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
}