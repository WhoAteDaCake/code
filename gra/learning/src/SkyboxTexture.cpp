#include "SkyboxTexture.h"

GLuint SkyboxTexture::load_cubemap(const char *files[6])
{
  GLuint textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

  int width, height, nrChannels;
  for (int i = 0; i < 6; i += 1)
  {
    std::string name = "./exports/";
    name += files[i];
    unsigned char *data = SOIL_load_image(name.c_str(), &width, &height, NULL, SOIL_LOAD_RGB);
    if (data)
    {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                   0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }
    else
    {
      std::cout << "Cubemap texture failed to load at path: " << files[i] << std::endl;
    }
    SOIL_free_image_data(data);
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  return textureID;
}

void SkyboxTexture::load()
{
  const char *files[6] = {
      "alpha-island_rt.tga",
      "alpha-island_lf.tga",
      "alpha-island_up.tga",
      "alpha-island_dn.tga",
      "alpha-island_ft.tga",
      "alpha-island_bk.tga",
  };
  this->id = load_cubemap(files);
}