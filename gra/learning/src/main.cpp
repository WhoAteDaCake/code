#include "Engine.h"

// Texture texture0(GL_TEXTURE_2D, 0);
// Texture texture1(GL_TEXTURE_2D, 1);
// Material material0;
// Mesh mesh0("test");
// Square square(0.5f);

// int window_w = 300;
// int window_h = 400;

// glm::vec3 cam_position(0.f, 0.f, 1.5f);
// glm::vec3 world_up(0.f, 1.f, 0.f);
// glm::vec3 cam_front(0.f, 0.f, -1.f);
// glm::mat4 view_matrix(1.f);
// // Projections
// float fov = 90.f;
// // Want this behind the camera to avoid clipping
// float near_plane = 0.1f;
// // Draw distance
// float far_plane = 100.f;
// glm::mat4 projection_matrix(1.f);

// Shaders shader;

// void Draw()
// {
//     glClearColor(0.f, 0.f, 0.f, 1.f);
//     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

//     // Lights
//     glm::vec3 light_pos0(0.f, 0.f, 1.f);
//     material0.send_to_shader(shader);

//     // Send transformation matrix (move, scale)
//     // shader.useM4fv("model_matrix", model_matrix);
//     shader.useM4fv("view_matrix", view_matrix);

//     projection_matrix = glm::mat4(1.f);
//     projection_matrix = glm::perspective(
//         glm::radians(fov),
//         static_cast<float>(window_w) / static_cast<float>(window_h),
//         near_plane,
//         far_plane);

//     shader.useM4fv("projection_matrix", projection_matrix);

//     // send camera
//     shader.use3fv("camera_pos", cam_position);

//     // Send light to the shaders
//     shader.use3fv("light_pos0", light_pos0);

//     shader.use();

//     texture0.bind();
//     texture1.bind();

//     square.render(&shader);

//     glutSwapBuffers();
//     glFlush();
//     Log::check_error("display");
//     // Reset
//     glBindVertexArray(0);
//     glUseProgram(0);

//     texture0.unbind();
//     texture1.unbind();

//     Log::check_error("reset");
// }

// void Initialize()
// {
//     glClearColor(0.f, 0.f, 0.f, 0.0);
//     // TEMP

//     square.initialize();

//     // glEnable(GL_DEBUG_OUTPUT);
//     // glDebugMessageCallback(MessageCallback, 0);

//     glViewport(0, 0, window_w, window_h);

//     Log::check_error("Initialize");
//     texture0.load("pusheen.png");
//     texture1.load("container.png");
//     material0.set_specs(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f), texture0.get_unit(), texture1.get_unit());

//     shader.set_shaders("vertex_core.glsl", "fragment_core.glsl", "");
// }

// void handle_key(unsigned char key, int x, int y)
// {
//     std::cout << "Key: " << key << std::endl;
//     float change = 0.1f;
//     glm::vec3 position = square.position;
//     glm::vec3 rotation = square.rotation;
//     // Escape key
//     if (key == 27)
//     {

//         glutLeaveMainLoop();
//     }
//     else if (key == 'w')
//     {
//         position.z -= change;
//     }
//     else if (key == 's')
//     {
//         position.z += change;
//     }
//     else if (key == 'a')
//     {
//         position.x -= change;
//     }
//     else if (key == 'd')
//     {
//         position.x += change;
//     }
//     else if (key == 'q')
//     {
//         rotation.y += change * 100;
//     }

//     square.position = position;
//     square.rotation = rotation;
//     square.update();

//     glutPostRedisplay();
// }

// void idle_func()
// {
//     glutPostRedisplay();
// }

// void reshape_func(int w, int h)
// {
//     window_w = w;
//     window_h = h;
//     // std::cout << "New sizes " << window_w << " " << window_h << std::endl;
//     glutPostRedisplay();
//     glViewport(0, 0, w, h);
// }

int main(int iArgc, char **cppArgv)
{
    Engine engine = Engine(iArgc, cppArgv, "Test window", 300, 300);

    engine.initialize();
    // glutInit(&iArgc, cppArgv);

    // glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
    // glutInitWindowSize(window_w, window_h);
    // glutInitWindowPosition(200, 200);
    // glutCreateWindow("My window");

    // // GL options
    // glEnable(GL_DEPTH_TEST);
    // // Not drawn if not seen
    // glEnable(GL_CULL_FACE);
    // glEnable(GL_COLOR_MATERIAL);

    // glEnable(GL_BLEND);
    // // Blend colors of polygons
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // // Back will not be shown
    // glCullFace(GL_BACK);
    // glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    // // Set front face as shown one
    // glFrontFace(GL_CCW);
    // // Set to fill the whole shape
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // Initialize();
    // glutDisplayFunc(Draw);
    // glutKeyboardFunc(handle_key);
    // glutReshapeFunc(reshape_func);

    // glutIdleFunc(idle_func);

    // glutMainLoop();

    return 0;
}