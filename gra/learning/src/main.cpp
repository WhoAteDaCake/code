#include "libs.h"

std::string shader(std::string file)
{
    return "./shaders/" + file;
}

void Draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
    glVertex3f(0.25, 0.25, 0.0);
    glVertex3f(0.75, 0.75, 0.0);
    glEnd();
    glFlush();
}

void Initialize()
{
    glClearColor(1.f, 0.4f, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

std::string load_file(std::string name)
{
    std::string temp = "";
    std::string src = "";

    std::ifstream in_file;

    in_file.open(name);

    if (in_file.is_open())
    {
        std::cout << "INFO: Loading file: " << name << "\n";
        while (std::getline(in_file, temp))
        {
            src += temp + "\n";
        }
    }
    else
    {
        std::cout << "ERROR:Could not load " << name << "\n";
    }
    in_file.close();
    return src;
}

bool load_shaders(GLuint &program)
{
    char logs[512];
    GLint success;
    std::string file = load_file(shader("vertex_core.glsl"));
    return true;
}

int main(int iArgc, char **cppArgv)
{
    int windowSize = 400;
    glutInit(&iArgc, cppArgv);

    GLuint program;
    load_shaders(program);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(windowSize, windowSize);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("XoaX.net");
    Initialize();
    glutDisplayFunc(Draw);
    glutMainLoop();
    return 0;
}