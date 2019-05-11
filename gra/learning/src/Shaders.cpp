#include "Shaders.h"

std::string Shaders::shader_src(std::string name)
{
  std::string full_path = "./shaders/" + name;
  std::string temp = "";
  std::string src = "";

  std::ifstream in_file;

  in_file.open(full_path);

  if (in_file.is_open())
  {
    Log::log("Loading file" + name + "\n");
    while (std::getline(in_file, temp))
    {
      src += temp + "\n";
    }
  }
  else
  {
    Log::error("Could not load " + name + "\n");
  }
  in_file.close();
  return src;
}

GLuint Shaders::load_shader(std::string name, GLenum type)
{
  char logs[512];
  GLint success;
  std::string src = shader_src(name);
  GLuint shader = glCreateShader(type);
  const GLchar *s_src = src.c_str();
  glShaderSource(shader, 1, &s_src, NULL);
  glCompileShader(shader);

  // Check whether it was successful
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(shader, 512, NULL, logs);
    std::string log_str(logs);
    Log::error("Could not compile vertex shader: " + name + "\n");
    Log::error("Logs: " + log_str + "\n");
    throw std::string("Failed to compile");
  }
  return shader;
}

void Shaders::bind_attrib(GLuint program)
{
  // Because of 1.30 glsl, we need to specify the order of attributes manually
  // https://stackoverflow.com/questions/21354301/glsl-syntax-problems-unexpected-new-identifier
  glBindAttribLocation(program, 0, "vertex_position");
  glBindAttribLocation(program, 1, "vertex_color");
  glBindAttribLocation(program, 2, "vertex_texcoord");
  glBindAttribLocation(program, 3, "vertex_normal");
}

GLuint Shaders::link_shaders(std::string vertex, std::string fragment, std::string geometry)
{
  GLuint vertex_shader = load_shader(vertex, GL_VERTEX_SHADER);
  GLuint fragment_shader = load_shader(fragment, GL_FRAGMENT_SHADER);
  GLuint geometry_shader = 0;

  if (geometry.size() != 0)
  {
    geometry_shader = load_shader(geometry, GL_GEOMETRY_SHADER);
  }

  Log::check_error("Load:shaders");
  // Allocate space
  GLuint program = glCreateProgram();

  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  if (geometry_shader)
  {
    glAttachShader(program, geometry_shader);
  }

  bind_attrib(program);

  glLinkProgram(program);
  Log::check_error("Shader link");

  // Check for errors
  char logs[512];
  GLint success;
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(program, 512, NULL, logs);
    std::string log_str(logs);
    Log::error("Could not link shaders program\n");
    Log::error("LOGS: " + log_str + "\n");
    throw std::string("Failed");
  }

  glUseProgram(0);
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
  if (geometry_shader)
  {
    glDeleteShader(geometry_shader);
  }
  Log::check_error("Shader cleanup");
  return program;
}