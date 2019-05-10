#include "FileLoader.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

std::string FileLoader::BASE_DIR = "./exports/";

bool hasSmoothingGroup(const tinyobj::shape_t &shape)
{
  for (size_t i = 0; i < shape.mesh.smoothing_group_ids.size(); i += 1)
  {
    if (shape.mesh.smoothing_group_ids[i] > 0)
    {
      return true;
    }
  }
  return false;
}

std::vector<Mesh *> FileLoader::load(std::string file_name, std::unique_ptr<TextureManager> &manager, std::vector<std::shared_ptr<Material>> &materials)
{
  std::string input_file = FileLoader::BASE_DIR + file_name;
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;

  std::string warn;
  std::string err;

  bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, input_file.c_str(), FileLoader::BASE_DIR.c_str());
  if (!warn.empty())
  {
    Log::warn(warn);
  }

  if (!err.empty())
  {
    Log::error(err);
  }

  if (!ret)
  {
    throw std::string("Could not read object file\n");
  }

  // TMP
  printf("# of vertices  = %d\n", (int)(attrib.vertices.size()) / 3);
  printf("# of normals   = %d\n", (int)(attrib.normals.size()) / 3);
  printf("# of texcoords = %d\n", (int)(attrib.texcoords.size()) / 2);
  printf("# of materials = %d\n", (int)materials.size());
  printf("# of shapes    = %d\n", (int)shapes.size());

  Log::log("Materials: " + std::to_string(materials.size()));

  for (size_t m = 0; m < materials.size(); m++)
  {
    tinyobj::material_t *mp = &materials[m];
    std::string diffuse_tex = mp->diffuse_texname;
    if (diffuse_tex.size() < 1)
    {
      continue;
    }
    manager->add(diffuse_tex, GL_TEXTURE_2D);
  }

  std::vector<Mesh *> meshes;
  for (size_t s = 0; s < shapes.size(); s++)
  {
    std::vector<Vertex> vertices_all;
    // TODO smoothing normals
    for (size_t f = 0; f < shapes[s].mesh.indices.size() / 3; f++)
    {
      Vertex vertices[3] = {Vertex(), Vertex(), Vertex()};
      GLuint indices[3] = {0, 0, 0};

      tinyobj::index_t idx0 = shapes[s].mesh.indices[3 * f + 0];
      tinyobj::index_t idx1 = shapes[s].mesh.indices[3 * f + 1];
      tinyobj::index_t idx2 = shapes[s].mesh.indices[3 * f + 2];

      int current_material_id = shapes[s].mesh.material_ids[f];

      float diffuse[3];
      for (size_t i = 0; i < 3; i++)
      {
        diffuse[i] = materials[current_material_id].diffuse[i];
      }

      // Texture coordinates
      if (
          attrib.texcoords.size() > 0 &&
          idx0.texcoord_index >= 0 &&
          idx1.texcoord_index >= 0 &&
          idx2.texcoord_index >= 0)
      {
        auto tc = attrib.texcoords;
        vertices[0].texcoord = glm::vec2(tc[2 * idx0.texcoord_index], tc[2 * idx0.texcoord_index + 1]);
        vertices[1].texcoord = glm::vec2(tc[2 * idx0.texcoord_index], tc[2 * idx0.texcoord_index + 1]);
        vertices[2].texcoord = glm::vec2(tc[2 * idx0.texcoord_index], tc[2 * idx0.texcoord_index + 1]);
      }
      // Vertices
      float v[3][3];
      for (int k = 0; k < 3; k += 1)
      {
        v[0][k] = attrib.vertices[3 * idx0.vertex_index + k];
        v[1][k] = attrib.vertices[3 * idx1.vertex_index + k];
        v[2][k] = attrib.vertices[3 * idx2.vertex_index + k];
      }
      for (int k = 0; k < 3; k += 1)
      {
        vertices[k].position = glm::vec3(v[k][0], v[k][1], v[k][2]);
      }

      float n[3][3];
      if (
          attrib.normals.size() > 0 &&
          idx0.normal_index >= 0 &&
          idx1.normal_index >= 0 &&
          idx2.normal_index >= 0)
      {
        // Normals
        for (int k = 0; k < 3; k += 1)
        {
          n[0][k] = attrib.normals[3 * idx0.normal_index + k];
          n[1][k] = attrib.normals[3 * idx1.normal_index + k];
          n[2][k] = attrib.normals[3 * idx2.normal_index + k];
        }
        for (int k = 0; k < 3; k += 1)
        {
          vertices[k].normal = glm::vec3(v[k][0], v[k][1], v[k][2]);
        }
      }

      float normal_factor = 0.2;
      float diffuse_factor = 1 - normal_factor;
      for (int k = 0; k < 3; k += 1)
      {
        float c[3] = {n[k][0] * normal_factor + diffuse[0] * diffuse_factor,
                      n[k][1] * normal_factor + diffuse[1] * diffuse_factor,
                      n[k][2] * normal_factor + diffuse[2] * diffuse_factor};
        float len2 = c[0] * c[0] + c[1] * c[1] + c[2] * c[2];
        if (len2 > 0.0f)
        {
          float len = sqrtf(len2);

          c[0] /= len;
          c[1] /= len;
          c[2] /= len;
        }
        vertices[k].color = glm::vec3(c[0], c[1], c[2]);
        vertices_all.push_back(vertices[k]);
      }
    }

    Mesh *mesh = new Mesh(shapes[s].name);
    mesh->vertices = vertices_all;
    for (int const &id : shapes[s].mesh.material_ids)
    {
      tinyobj::material_t *mp = &materials[id];
      std::string diffuse_tex = mp->diffuse_texname;
      if (diffuse_tex.size() < 1)
      {
        continue;
      }
      mesh->texture_name = diffuse_tex;
    }
    meshes.push_back(mesh);
  }
  return meshes;
}