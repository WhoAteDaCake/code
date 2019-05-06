
#include "Engine.h"

std::unique_ptr<Engine> active;

void idle()
{
    active->idle_cb();
}

void reshape(int width, int height)
{
    active->reshape_cb(width, height);
}

void handle_key(unsigned char key, int x, int y)
{
    active->handle_key_cb(key, x, y);
}

void draw()
{
    active->draw_cb();
}

void message(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
    active->message_cb(source, type, id, severity, length, message, userParam);
}

// Called when a mouse button event occurs
void mouse_button(int button, int state, int x, int y)
{
    active->mouse_button_cb(button, state, x, y);
}

void mouse_move(int x, int y)
{
    active->mouse_move_cb(x, y);
}

int main(int iArgc, char **cppArgv)
{
    active = std::unique_ptr<Engine>(new Engine(iArgc, cppArgv, "Test window", 400, 400));

    try
    {
        glutDisplayFunc(draw);
        glutKeyboardFunc(handle_key);
        glutReshapeFunc(reshape);
        glutIdleFunc(idle);
        glutMouseFunc(mouse_button);
        glutMotionFunc(mouse_move);

#ifdef GRA_DEBUG
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(message, 0);
#endif // DEBUG

        active->initialize();
    }
    catch (std::string message)
    {
        std::cout << message << "\n";
    }

    return 0;
}