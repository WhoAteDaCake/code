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

GLuint load_shaders(std::string name, GLenum type)
{
    char logs[512];
    GLint success;
    std::string src = load_file(shader(name));
    GLuint shader = glCreateShader(type);
    const GLchar *s_src = src.c_str();
    glShaderSource(shader, 1, &s_src, NULL);
    glCompileShader(shader);

    // Check whether it was successful
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == -1)
    {
        glGetShaderInfoLog(shader, 512, NULL, logs);
        std::cout << "ERROR: could not compile vertex shader :" << name << std::endl;
        std::cout << logs << std::endl;
    }
    return shader;
}

bool load_shaders(GLuint &program)
{
    GLuint vertex_shader = load_shaders("vertex_core.glsl", GL_VERTEX_SHADER);
    GLuint fragment_shader = load_shaders("fragment_core.glsl", GL_FRAGMENT_SHADER);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
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