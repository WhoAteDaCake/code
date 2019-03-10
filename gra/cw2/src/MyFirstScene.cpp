#include "MyFirstScene.h"

void draw()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear buffers
	glMatrixMode(GL_MODELVIEW);													// set for model and viewing operations
	glLoadIdentity();
	glClearColor(1.f, 1.f, 1.f, 1.f); // set background colour
	glTranslatef(0.f, 0.f, -300.f);		// reset drawing
	// Rotate when user changes rotate_x and rotate_y

	glRotatef(rotate_x, 1.0, 0.0, 0.0);
	glRotatef(rotate_y, 0.0, 1.0, 0.0);
	// move drawing further back in the scene

	draw_octahedron();
	if (MojaveWorkAround)
	{
		glutReshapeWindow(601, 401);
		MojaveWorkAround = false;
	}
	glutPostRedisplay();
	checkGLError();
	glutSwapBuffers(); // execute all commands, swap buffers
}

void setup()
{
	width = 600;																	 // initialise global window variables
	height = 400;																	 // define in your header: int width, height;
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE); // enable 3D rendering and double buffering
	glutInitWindowSize(width, height);						 // set window size
	glutCreateWindow("My Scene");									 // create and show window (named MyScene)
	draw();
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
	int e = 0;									 // Error count
	GLenum error = glGetError(); // Get first glError
	while (GL_NO_ERROR != error) // Loop until no errors found
	{
		e++;
		printf("GL Error %i: %s\n", e, gluErrorString(error)); // Display error string
		error = glGetError();																	 // Get next glError
	}
}

void specialKeys(int key, int x, int y)
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
	glutInit(&argc, argv); // Initialise GL environment

	setup(); // Call additional initialisation commands
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glutDisplayFunc(draw); // Register scene to render contents of draw() function
	checkGLError();				 // Check any OpenGL errors in initialisation
	glutReshapeFunc(reshape);
	glutSpecialFunc(specialKeys);
	glEnable(GL_DEPTH_TEST); // Enable depth testing
	glDepthMask(GL_TRUE);		 // Enable depth write
	glDepthFunc(GL_LEQUAL);	// Choose the depth comparison function

	glutMainLoop(); // Begin rendering sequence
	setup();
	return 0;
}
