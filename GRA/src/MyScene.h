#ifndef ___MyScene__
#define ___MyScene__
#include <GL/glut.h>

#include <stdio.h>

void setup();
void draw();
void checkGLError(); // Prints any OpenGL errors to console
int main(int argc, char **argv);

#endif