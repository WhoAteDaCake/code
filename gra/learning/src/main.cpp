#include "libs.h"

GLuint program;
GLuint VAO;
GLuint VBO;
GLuint EBO;
GLuint texture0;

/* report GL errors, if any, to stderr */
void checkError(const char *functionName)
{
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR)
    {
        fprintf(stderr, "GL error 0x%X detected in %s\n", error, functionName);
    }
}
float size = 0.5f;

Vertex vertices[] = {glm::vec3(-size, size, 0.f), glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.f, 1.f),  //
                     glm::vec3(-size, -size, 0.f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(0.f, 0.f), //
                     glm::vec3(size, -size, 0.f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(1.f, 0.f),  //
                     glm::vec3(size, size, 0.f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(1.f, 1.f)};

unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

GLuint indices[] = {0, 1, 2, // Triangle 1
                    0, 2, 3};
unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

std::string shader(std::string file) { return "./shaders/" + file; }

void Draw()
{
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glUseProgram(program);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture0);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, nrOfIndices, GL_UNSIGNED_INT, 0);

    glutSwapBuffers();
    glFlush();
    checkError("display");
}

void Initialize()
{
    glClearColor(0.f, 0.f, 0.f, 0.0);
    // glMatrixMode(GL_PROJECTION);
    // glLoadIdentity();
    // glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
    // MODEL

    // VAO, VBO, EBO
    // GEN VAO AND BIND
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // //GEN VBO AND BIND AND SEND DATA
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // //GEN EBO AND BIND AND SEND DATA
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

    // SET VERTEXATTRIBPOINTERS AND ENABLE (INPUT ASSEMBLY)
    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid *)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    // Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid *)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);
    // Texcoord
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid *)offsetof(Vertex, texcoord));
    glEnableVertexAttribArray(2);

    // BIND VAO 0
    glBindVertexArray(0);

    checkError("Initialize");

    // Textures
    int image_w = 0;
    int image_h = 0;
    unsigned char *image = SOIL_load_image("./images/robot.png", &image_w, &image_h, NULL, SOIL_LOAD_RGBA);

    // Make sure previous texture is saved
    GLuint boundTexture = 0;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint *)&boundTexture);

    glGenTextures(1, &texture0);
    glBindTexture(GL_TEXTURE_2D, texture0);
    checkError("Texture:bind");

    // Set options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    checkError("Texture:params");

    if (image)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_w, image_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        // For automatic adjustment of size
        glGenerateMipmap(GL_TEXTURE_2D);
        checkError("Texture:image");
    }
    else
    {
        std::cout << "ERROR: failed to load texture\n";
        return;
    }

    SOIL_free_image_data(image);
    // Make sure no textures are bound
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, boundTexture);
    checkError("Texture:reset");
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
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, logs);
        std::cout << "ERROR: could not compile vertex shader :" << name
                  << std::endl;
        std::cout << logs << std::endl;
    }
    return shader;
}

bool load_shaders(GLuint &program)
{
    GLuint vertex_shader = load_shaders("vertex_core.glsl", GL_VERTEX_SHADER);
    GLuint fragment_shader =
        load_shaders("fragment_core.glsl", GL_FRAGMENT_SHADER);

    // Allocate space
    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);

    // Because of 1.30 glsl, we need to specify attributes manually
    // https://stackoverflow.com/questions/21354301/glsl-syntax-problems-unexpected-new-identifier
    glBindAttribLocation(program, 0, "vertex_position");
    glBindAttribLocation(program, 1, "vertex_color");
    glBindAttribLocation(program, 2, "vertex_texcoord");

    glLinkProgram(program);

    // Check for errors
    char logs[512];
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(program, 512, NULL, logs);
        std::cout << "ERROR: could not link program \n";
        std::cout << logs << std::endl;
    }

    glUseProgram(0);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    return true;
}

void handle_key(unsigned char key, int x, int y)
{
    // Escape key
    if (key == 27)
    {
        glutLeaveMainLoop();
    }
}

int main(int iArgc, char **cppArgv)
{
    int windowSize = 400;
    glutInit(&iArgc, cppArgv);

    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowSize(windowSize, windowSize);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("XoaX.net");

    load_shaders(program);

    // GL options
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    // Blend colors of polygons
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glCullFace(GL_BACK);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    // Set front face as shown one
    glFrontFace(GL_CCW);
    // Set to fill the whole shape
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    Initialize();
    glutDisplayFunc(Draw);
    glutKeyboardFunc(handle_key);

    glutMainLoop();
    glDeleteProgram(program);

    return 0;
}