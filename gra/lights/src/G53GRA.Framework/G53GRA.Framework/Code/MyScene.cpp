#include "MyScene.h"

#include "Wall.h"
#include "Light.h"

MyScene::MyScene(int argc, char **argv, const char *title, const int &windowWidth, const int &windowHeight)
		: Scene(argc, argv, title, windowWidth, windowHeight)
{
}

void MyScene::Initialise()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Wall
	float wall_z = -200.f;
	float wall_w = 600.f;

	Wall *w = new Wall();
	w->position(0.0f, 0.0f, wall_z);
	w->size(800.0f, wall_w, 1.0f);

	float lamp_z = wall_z / 2.f;
	float offset = wall_w / 4.f;
	// Light A (red)

	// static GLfloat ambient_A[] = {0.5f, 0.0f, 0.0f, 1.0f};
	// static GLfloat diffuse_A[] = {0.9f, 0.0f, 0.0f, 1.0f};
	// static GLfloat specular_A[] = {0.0f, 0.0f, 0.0f, 1.0f};
	// static GLfloat position_A[] = {-offset, 0.0f, lamp_z, 1.0f};

	// Light *l_A = new Light(GL_LIGHT0, ambient_A, diffuse_A, specular_A, position_A);

	// // Light B (blue)

	// static GLfloat ambient_B[] = {0.0f, 0.0f, 0.5f, 1.0f};
	// static GLfloat diffuse_B[] = {0.0f, 0.0f, 0.9f, 1.0f};
	// static GLfloat specular_B[] = {0.0f, 0.0f, 0.0f, 1.0f};
	// static GLfloat position_B[] = {offset, 0.0f, lamp_z, 1.0f};

	// Light *l_B = new Light(GL_LIGHT1, ambient_B, diffuse_B, specular_B, position_B);

	AddObjectToScene(w);
	AddObjectToScene(new Light(GL_LIGHT0, offset, lamp_z, 0));
	AddObjectToScene(new Light(GL_LIGHT1, -offset, lamp_z, 2));
}

void MyScene::Projection()
{
	GLdouble aspect = static_cast<GLdouble>(windowWidth) / static_cast<GLdouble>(windowHeight);
	gluPerspective(60.0, aspect, 1.0, 1000.0);
}
