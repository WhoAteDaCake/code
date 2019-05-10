#include "FileLoader.h"

/**
 * Some of the loader code has been implemented by using
 * https://github.com/syoyo/tinyobjloader/tree/master/examples/viewer
 */
#define loop3(x) for (int x = 0; x < 3; ++x)

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

std::string FileLoader::BASE_DIR = "./exports/";

glm::vec3 to_vec(const tinyobj::real_t entry[3])
{
  return glm::vec3(entry[0], entry[1], entry[2]);
}

glm::vec3 get_normal(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3)
{
  return glm::triangleNormal(v1, v2, v3);
}

bool has_smoothing(const tinyobj::shape_t &shape)
{
  for (auto i : shape.mesh.smoothing_group_ids)
  {
    if (i > 0)
    {
      return true;
    }
  }
  return false;
}

// int vi_i(int entry, int offset)
// {
//   return 3 * entry + offset;
// }

// std::map<int, glm::vec3> get_smoothing_normals(const tinyobj::attrib_t &attrib, const tinyobj::shape_t &shape)
// {
//   std::map<int, glm::vec3> s_normals;
//   if (!has_smoothing(shape))
//   {
//     return s_normals;
//   }

//   int vertices_n = attrib.vertices.size();

//   for (size_t f = 0; f < shape.mesh.indices.size() / 3; f++)
//   {
//     tinyobj::index_t idx0 = shape.mesh.indices[vi_i(f, 0)];
//     tinyobj::index_t idx1 = shape.mesh.indices[vi_i(f, 1)];
//     tinyobj::index_t idx2 = shape.mesh.indices[vi_i(f, 2)];
//     int vi[3];     // indexes
//     float v[3][3]; // coordinates

//     for (int k = 0; k < 3; k++)
//     {
//       vi[0] = idx0.vertex_index;
//       vi[1] = idx1.vertex_index;
//       vi[2] = idx2.vertex_index;
//       int v1 = vi_i(vi[0], k);
//       int v2 = vi_i(vi[1], k);
//       int v3 = vi_i(vi[2], k);
//       // Validate indices
//       assert(vi[0] >= 0);
//       assert(vi[1] >= 0);
//       assert(vi[2] >= 0);
//       assert(v1 < vertices_n);
//       assert(v2 < vertices_n);
//       assert(v3 < vertices_n);

//       v[0][k] = attrib.vertices[v1];
//       v[1][k] = attrib.vertices[v2];
//       v[2][k] = attrib.vertices[v3];
//     }

//     // Compute the normal of the face
//     glm::vec3 normal = get_normal(to_vec(v[0]), to_vec(v[1]), to_vec(v[2]));

//     // Add the normal to the three vertexes
//     for (size_t i = 0; i < 3; ++i)
//     {
//       auto iter = s_normals.find(vi[i]);
//       if (iter != s_normals.end())
//       {
//         iter->second += normal;
//       }
//       else
//       {
//         iter->second = normal;
//       }
//     }
//   }
//   return s_normals;
// }

void register_materials(std::unique_ptr<MaterialManager> &mat_manager, std::vector<tinyobj::material_t> &materials)
{
#ifdef GRA_DEBUG
  Log::log("MATERIALS:total:" + std::to_string(materials.size()));
#endif // DEBUG
  for (auto const &material : materials)
  {
    // TODO load textures in the future
    if (material.diffuse_texname.size() != 0)
    {
      throw std::string("Didn't load existing texture");
    }
#ifdef GRA_DEBUG
    Log::log("MATERIALS:adding:" + material.name);
#endif // DEBUG
    Material *mat = new Material(
        to_vec(material.ambient),
        to_vec(material.diffuse),
        to_vec(material.specular),
        false);
    mat_manager->add(material.name, mat);
  }
}

std::vector<Mesh *> FileLoader::load(
    std::string file_name,
    std::unique_ptr<TextureManager> &tex_manager,
    std::unique_ptr<MaterialManager> &mat_manager)
{
  std::string input_file = FileLoader::BASE_DIR + file_name;
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;

  tinyobj::material_t default_mat = tinyobj::material_t();
  default_mat.name = "default";
  // Append `default` material
  materials.push_back(default_mat);

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

  register_materials(mat_manager, materials);
  std::vector<Mesh *> meshes;

  for (size_t s = 0; s < shapes.size(); s++)
  {
    std::vector<Vertex> vertices_all;
    // TODO smoothing normals
    // std::map<int, glm::vec3> smoothing = get_smoothing_normals(attrib, shapes[s]);
    for (size_t f = 0; f < shapes[s].mesh.indices.size() / 3; f++)
    {
      Vertex vertices[3] = {Vertex(), Vertex(), Vertex()};
      GLuint indices[3] = {0, 0, 0};
      tinyobj::index_t ids[3];

      loop3(i)
      {
        ids[i] = shapes[s].mesh.indices[3 * f + i];
      }

      int mat_id = shapes[s].mesh.material_ids[f];
      if (mat_id < 0 || mat_id > materials.size())
      {
#ifdef GRA_DEBUG
        Log::warn("Could not find material for: " + shapes[s].name);
#endif // DEBUG
        mat_id = 0;
      }
      float diffuse[3];
      loop3(i)
      {
        diffuse[i] = materials[mat_id].diffuse[i];
      }

      auto tc = attrib.texcoords;
      // Texture coordinates
      if (
          tc.size() > 0 &&
          ids[0].texcoord_index >= 0 &&
          ids[1].texcoord_index >= 0 &&
          ids[2].texcoord_index >= 0)
      {
        loop3(i)
        {
          vertices[i].texcoord = glm::vec2(tc[2 * ids[i].texcoord_index], tc[2 * ids[i].texcoord_index + 1]);
        }
      }
      // Vertices
      float v[3][3];
      loop3(i)
      {
        loop3(k)
        {
          v[k][i] = attrib.vertices[3 * ids[k].vertex_index + i];
        }
      }
      loop3(i)
      {
        vertices[i].position = glm::vec3(v[i][0], v[i][1], v[i][2]);
      }

      float n[3][3];
      if (
          attrib.normals.size() > 0 &&
          ids[0].normal_index >= 0 &&
          ids[1].normal_index >= 0 &&
          ids[2].normal_index >= 0)
      {
        // Normals
        loop3(k)
        {
          loop3(i)
          {
            n[i][k] = attrib.normals[3 * ids[i].normal_index + k];
          }
        }
        loop3(k)
        {
          vertices[k].normal = glm::vec3(n[k][0], n[k][1], n[k][2]);
        }
      }
      else
      {
#ifdef GRA_DEBUG
        Log::warn("No normals for " + shapes[s].name);
#endif
        // loop3(i) {
        //   vetrices[]
        // }
      }

      float normal_factor = 0.2;
      float diffuse_factor = 1 - normal_factor;
      loop3(k)
      {
        float c[3];
        loop3(i)
        {
          c[i] = n[k][i] * normal_factor + diffuse[i] * diffuse_factor;
        }
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
    // Use the first material id
    auto mat_ids = shapes[s].mesh.material_ids;
    std::string mat_name = "default";
    if (mat_ids.size() > 0)
    {
      mat_name = materials[mat_ids[0]].name;
    }

    Mesh *mesh = new Mesh(shapes[s].name);
    mesh->vertices = vertices_all;
    mesh->mat_name = mat_name;
    meshes.push_back(mesh);
  }
  return meshes;
}