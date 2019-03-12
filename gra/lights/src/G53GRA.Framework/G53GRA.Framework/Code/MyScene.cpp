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
	AddObjectToScene(w);
	AddObjectToScene(new Light(GL_LIGHT0, offset, lamp_z, 2));
	AddObjectToScene(new Light(GL_LIGHT1, -offset, lamp_z, 0));
}

void MyScene::Projection()
{
	GLdouble aspect = static_cast<GLdouble>(windowWidth) / static_cast<GLdouble>(windowHeight);
	gluPerspective(60.0, aspect, 1.0, 1000.0);
}
