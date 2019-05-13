#include "Texture.h"

GLuint Texture::load_texture(std::string file, GLenum type)
{

  std::string log_prefix = "Texture:" + file;
  std::string full_path = "./images/" + file;
  // Textures
  unsigned char *image = SOIL_load_image(full_path.c_str(), &this->width, &this->height, NULL, SOIL_LOAD_RGBA);

  // Make sure previous texture is saved
  GLuint new_texture;

  glGenTextures(1, &new_texture);
  glBindTexture(type, new_texture);
  Log::check_error(log_prefix + ":bind");

  // Set options
  glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  Log::check_error(log_prefix + ":params");

  if (image)
  {
    glTexImage2D(type, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    // For automatic adjustment of size
    glGenerateMipmap(type);
    Log::check_error(log_prefix + ":image");
    Log::log("Loaded texture: " + file + "\n");
  }
  else
  {
    Log::error("Could not load texure: " + file + "\n");
    throw std::string("Failed to load the image");
    return 0;
  }

  SOIL_free_image_data(image);
  Log::check_error(log_prefix + ":reset");
  return new_texture;
}