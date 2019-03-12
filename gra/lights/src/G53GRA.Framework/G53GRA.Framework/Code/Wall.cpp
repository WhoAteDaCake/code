#include "Wall.h"

GLfloat ambient[] = {0.5f, 0.5f, 0.5f, 1.0f};
GLfloat diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat specular[] = {0.0f, 0.0f, 0.0f, 1.0f};

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

Wall::Wall() {}

void Wall::Display()
{
  glPushMatrix();
  glPushAttrib(GL_ALL_ATTRIB_BITS);

  glTranslatef(pos[0], pos[1], pos[2]);
  glScalef(scale[0], scale[1], scale[2]);

  // HAS TO RUN AFTER PUSH AND TRANSLATE
  // OTHERWISE WILL GET HORRIBLE SIDE EFFECTS
  glDisable(GL_COLOR_MATERIAL);
  glEnable(GL_LIGHTING);

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 1.f);

  // Best ratio, otherwise it seems to freak out and not render the view
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
  glPopAttrib();
  glPopMatrix();
}