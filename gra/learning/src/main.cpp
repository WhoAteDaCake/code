#include "libs.h"
#include "Shaders.h"
#include "Texture.h"

GLuint VAO;
GLuint VBO;
GLuint EBO;
Texture texture(GL_TEXTURE_2D, 0);
int window_w = 300;
int window_h = 400;

glm::vec3 position(0.f);
glm::vec3 scale(1.f);
glm::vec3 rotation(0.f);
glm::mat4 model_matrix(1.f);
// View matrix
glm::vec3 cam_position(0.f, 0.f, 1.5f);
glm::vec3 world_up(0.f, 1.f, 0.f);
glm::vec3 cam_front(0.f, 0.f, -1.f);
glm::mat4 view_matrix(1.f);
// Projections
float fov = 90.f;
// Want this behind the camera to avoid clipping
float near_plane = 0.1f;
// Draw distance
float far_plane = 100.f;
glm::mat4 projection_matrix(1.f);

Shaders shader;

float size = 0.5f;

Vertex vertices[] = {
    glm::vec3(-size, size, 0.f),
    glm::vec3(1.f, 0.f, 0.f),
    glm::vec2(0.f, 1.f),
    glm::vec3(0.f, 0.f, -1.f), // Part2
    glm::vec3(-size, -size, 0.f),
    glm::vec3(0.f, 1.f, 0.f),
    glm::vec2(0.f, 0.f),
    glm::vec3(0.f, 0.f, -1.f), // Part3
    glm::vec3(size, -size, 0.f),
    glm::vec3(0.f, 0.f, 1.f),
    glm::vec2(1.f, 0.f),
    glm::vec3(0.f, 0.f, -1.f), // Part4
    glm::vec3(size, size, 0.f),
    glm::vec3(0.f, 0.f, 1.f),
    glm::vec2(1.f, 1.f),
    glm::vec3(0.f, 0.f, -1.f),
};

unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

GLuint indices[] = {
    0, 1, 2, // Triangle 1
    0, 2, 3};
unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

void Draw()
{
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // Lights
    glm::vec3 light_pos0(0.f, 0.f, 1.f);

    model_matrix = glm::mat4(1.f);
    model_matrix = glm::translate(model_matrix, position);
    model_matrix = glm::rotate(model_matrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.f, 0.f));
    model_matrix = glm::rotate(model_matrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.f, 0.f));
    model_matrix = glm::rotate(model_matrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.f, 1.f));
    model_matrix = glm::scale(model_matrix, scale);

    // Update uniforms(if you need more than 1 texture)
    shader.use1i("texture0", texture.get_unit());
    // Send transformation matrix (move, scale)
    shader.useM4fv("model_matrix", model_matrix);
    shader.useM4fv("view_matrix", view_matrix);

    projection_matrix = glm::mat4(1.f);
    projection_matrix = glm::perspective(
        glm::radians(fov),
        static_cast<float>(window_w) / static_cast<float>(window_h),
        near_plane,
        far_plane);

    shader.useM4fv("projection_matrix", projection_matrix);

    // send camera
    shader.use3fv("camera_pos", cam_position);

    // Send light to the shaders
    shader.use3fv("light_pos0", light_pos0);

    shader.use();

    texture.bind();

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, nrOfIndices, GL_UNSIGNED_INT, 0);

    glutSwapBuffers();
    glFlush();
    Log::check_error("display");
    // Reset
    glBindVertexArray(0);
    glUseProgram(0);
    texture.unbind();
    Log::check_error("reset");
}

void Initialize()
{
    glClearColor(0.f, 0.f, 0.f, 0.0);

    /*
     VAO,
     VBO - Vertex buffer object (send positions of vertices)
     EBO - Element buffer object (send indices of vertices)
    */

    // GEN VAO AND BIND
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // //GEN VBO AND BIND AND SEND DATA
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // GL_STATIC_DRAW because we won't modify the values
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    // //GEN EBO AND BIND AND SEND DATA
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_DYNAMIC_DRAW);

    // SET VERTEXATTRIBPOINTERS AND ENABLE (INPUT ASSEMBLY)
    // Position
    // Could use glGetAtrributeLocation
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid *)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    // Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid *)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);
    // Texture coord
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid *)offsetof(Vertex, texcoord));
    glEnableVertexAttribArray(2);
    // Normals
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid *)offsetof(Vertex, normal));
    glEnableVertexAttribArray(3);

    // BIND VAO 0
    glBindVertexArray(0);

    glViewport(0, 0, window_w, window_h);

    Log::check_error("Initialize");
    texture.load("robot.png");

    view_matrix = glm::lookAt(cam_position, cam_position + cam_front, world_up);
    projection_matrix = glm::perspective(
        glm::radians(fov),
        static_cast<float>(window_w) / static_cast<float>(window_h),
        near_plane,
        far_plane);

    shader.set_shaders("vertex_core.glsl", "fragment_core.glsl", "");
}

void handle_key(unsigned char key, int x, int y)
{
    std::cout << "Key: " << key << std::endl;
    float change = 0.1f;
    // Escape key
    if (key == 27)
    {

        glutLeaveMainLoop();
    }
    else if (key == 'w')
    {
        position.z -= change;
    }
    else if (key == 's')
    {
        position.z += change;
    }
    else if (key == 'a')
    {
        position.x -= change;
    }
    else if (key == 'd')
    {
        position.x += change;
    }
    else if (key == 'q')
    {
        rotation.y += change * 100;
    }

    glutPostRedisplay();
}

void idle_func()
{
    glutPostRedisplay();
}

void reshape_func(int w, int h)
{
    window_w = w;
    window_h = h;
    // std::cout << "New sizes " << window_w << " " << window_h << std::endl;
    glutPostRedisplay();
    glViewport(0, 0, w, h);
}

int main(int iArgc, char **cppArgv)
{
    glutInit(&iArgc, cppArgv);

    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowSize(window_w, window_h);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("My window");

    // GL options
    glEnable(GL_DEPTH_TEST);
    // Not drawn if not seen
    glEnable(GL_CULL_FACE);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_BLEND);
    // Blend colors of polygons
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // Back will not be shown
    glCullFace(GL_BACK);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    // Set front face as shown one
    glFrontFace(GL_CCW);
    // Set to fill the whole shape
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    Initialize();
    glutDisplayFunc(Draw);
    glutKeyboardFunc(handle_key);
    glutReshapeFunc(reshape_func);

    glutIdleFunc(idle_func);

    glutMainLoop();

    return 0;
}