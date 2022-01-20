#include <frame/scene/object/builtin_objects.hpp>
#include "obj_loader.hpp"

using namespace LE;

model_t::model_t(std::string const& path) : path_(path) {}

glm::vec3 model_t::bbox_min_pt() const { return bbox_min_pt_; }
glm::vec3 model_t::bbox_max_pt() const { return bbox_max_pt_; }

std::vector<object_ptr_t> model_t::compile() {
   objl::Loader loader;
   loader.LoadFile(path_);

   std::vector<object_ptr_t> result(loader.LoadedMeshes.size());
   std::map<std::string, texture_ptr_t> textures_map;
   
   bool bbox_initialised = false;
   size_t mesh_id = 0;
   for (auto const& mesh : loader.LoadedMeshes) {
      std::vector<glm::vec3> vertices(mesh.Vertices.size()), normales(mesh.Vertices.size());
      std::vector<glm::vec2> tex_coords(mesh.Vertices.size());

      for (size_t i = 0; i < mesh.Vertices.size(); ++i) {
         vertices[i] = mesh.Vertices[i].Position;
         normales[i] = mesh.Vertices[i].Normal;
         tex_coords[i] = glm::vec2(mesh.Vertices[i].TextureCoordinate.x, 1.f - mesh.Vertices[i].TextureCoordinate.y);

         if (!bbox_initialised) {
            bbox_min_pt_ = vertices[i];
            bbox_max_pt_ = vertices[i];
            bbox_initialised = true;
            continue;
         }
         
         for (size_t d_idx = 0; d_idx < 3; ++d_idx) {
            if (vertices[i][d_idx] < bbox_min_pt_[d_idx])
               bbox_min_pt_[d_idx] = vertices[i][d_idx];
            if (vertices[i][d_idx] > bbox_max_pt_[d_idx])
               bbox_max_pt_[d_idx] = vertices[i][d_idx];
         }
      }

      buffer_ptr_t buffer = std::make_shared<buffer_t>(vertices, normales, tex_coords);;
      buffer->add_index_buffer(mesh.Indices);

      std::vector<texture_ptr_t> textures;
      if (mesh.MeshMaterial.map_Kd.length() > 0) {
         if (textures_map.find(mesh.MeshMaterial.map_Kd) == textures_map.end()) {
            std::filesystem::path texture_path(path_);
            texture_path.remove_filename();
            texture_path += mesh.MeshMaterial.map_Kd;

            const texture_ptr_t texture = std::make_shared<texture_t>(std::make_shared<image_t>(texture_path.string()));
            textures.push_back(texture);
            textures_map[mesh.MeshMaterial.map_Kd] = texture;
         }
         else
            textures.push_back(textures_map[mesh.MeshMaterial.map_Kd]);
      }

      if (mesh.MeshMaterial.map_d.length() > 0) {
         if (textures_map.find(mesh.MeshMaterial.map_d) == textures_map.end()) {
            std::filesystem::path texture_path(path_);
            texture_path.remove_filename();
            texture_path += mesh.MeshMaterial.map_d;

            const texture_ptr_t texture = std::make_shared<texture_t>(std::make_shared<image_t>(texture_path.string()));
            textures.push_back(texture);
            textures_map[mesh.MeshMaterial.map_d] = texture;
         }
         else
            textures.push_back(textures_map[mesh.MeshMaterial.map_d]);
      }

      result[mesh_id] = std::make_shared<object_t>(buffer, shader_prog_t::create_lighted_and_textured(textures.size() > 1), textures);
      result[mesh_id++]->set_drawing_style(object_t::DS_triangles);
   }

   return result;
}
