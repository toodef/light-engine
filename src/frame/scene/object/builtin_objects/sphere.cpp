#define _USE_MATH_DEFINES
#include<cmath>

#include <frame/scene/object/builtin_objects.hpp>

using namespace LE;

sphere_t::sphere_t(glm::vec3 const& center, float radius, size_t detalisation) : center_(center), radius_(radius), detalisation_(detalisation)  {}

void sphere_t::set_shader_prog(shader_prog_ptr_t const & prog) { shader_prog_ = prog; }
void sphere_t::set_texture(texture_ptr_t const & texture) { texture_ = texture; }
void sphere_t::set_color(glm::vec3 const& color) { color_ = color; color_was_set_ = true; }

void sphere_t::generate_tex_coords() { generate_tex_coords_ = true; }

object_ptr_t sphere_t::compile() const {
   std::vector<glm::vec3> vertices;
   std::vector<unsigned int> indices;
   generate_vertices_(vertices, indices);

   buffer_ptr_t buffer;
   if (generate_tex_coords_) {
      std::vector<glm::vec2> tex_coords(vertices.size());
      for (unsigned int i = 0; i < vertices.size(); ++i) {
         tex_coords[i] = glm::vec2(atan2f(vertices[i].x, vertices[i].z) / (2 * M_PI) + 0.5f, 0.5f - asinf(vertices[i].y) / M_PI);
         vertices[i] = vertices[i] * radius_ + center_;
      }

      if (color_was_set_)
         buffer = std::make_shared<buffer_t>(vertices, color_, tex_coords);
      else
         buffer = std::make_shared<buffer_t>(vertices, tex_coords);
   }
   else {
      for (unsigned int i = 0; i < vertices.size(); ++i)
         vertices[i] = vertices[i] * radius_ + center_;

      if (color_was_set_)
         buffer = std::make_shared<buffer_t>(vertices, color_);
      else
         buffer = std::make_shared<buffer_t>(vertices);
   }

   buffer->add_index_buffer(indices);
   auto object = std::make_shared<object_t>(buffer, shader_prog_, texture_);
   object->set_drawing_style(object_t::DS_triangles);
   return object;
}

void sphere_t::generate_vertices_(std::vector<glm::vec3> & vertices, std::vector<unsigned int> & indices) const {
   const float x = 0.525731112119133606f, z = 0.850650808352039932f;

   vertices = {
      glm::normalize(glm::vec3(-x, 0, z)),
      glm::normalize(glm::vec3(x, 0, z)),
      glm::normalize(glm::vec3(-x, 0, -z)),
      glm::normalize(glm::vec3(x, 0, -z)),
      glm::normalize(glm::vec3(0, z, x)),
      glm::normalize(glm::vec3(0, z, -x)),
      glm::normalize(glm::vec3(0, -z, x)),
      glm::normalize(glm::vec3(0, -z, -x)),
      glm::normalize(glm::vec3(z, x, 0)),
      glm::normalize(glm::vec3(-z, x, 0)),
      glm::normalize(glm::vec3(z, -x, 0)),
      glm::normalize(glm::vec3(-z, -x, 0))
   };

   std::vector<unsigned int> index_data;
   indices = {
      0 , 4 , 1 ,
      0 , 9 , 4 ,
      9 , 5 , 4 ,
      4 , 5 , 8 ,
      4 , 8 , 1 ,
      8 , 10, 1 ,
      8 , 3 , 10,
      5 , 3 , 8 ,
      5 , 2 , 3 ,
      2 , 7 , 3 ,
      7 , 10, 3 ,
      7 , 6 , 10,
      7 , 11, 6 ,
      11, 0 , 6 ,
      0 , 1 , 6 ,
      6 , 1 , 10,
      9 , 0 , 11,
      9 , 11, 2 ,
      9 , 2 , 5 ,
      7 , 2 , 11 };

   for (size_t n = 0; n < detalisation_ - 1; ++n)
   {
      for (size_t i = 0; i < indices.size() - 2; i += 3)
      {
         size_t i0 = indices[i];
         size_t i1 = indices[i + 1];
         size_t i2 = indices[i + 2];

         glm::vec3 v_m01 = glm::normalize(vertices[i0] + vertices[i1]);
         glm::vec3 v_m12 = glm::normalize(vertices[i1] + vertices[i2]);
         glm::vec3 v_m02 = glm::normalize(vertices[i0] + vertices[i2]);

         size_t m01 = vertices.size();
         vertices.push_back(v_m01);

         size_t m12 = vertices.size();
         vertices.push_back(v_m12);

         size_t m02 = vertices.size();
         vertices.push_back(v_m02);

         index_data.push_back(i0);
         index_data.push_back(m01);
         index_data.push_back(m02);
         index_data.push_back(i1);
         index_data.push_back(m12);
         index_data.push_back(m01);
         index_data.push_back(i2);
         index_data.push_back(m02);
         index_data.push_back(m12);
         index_data.push_back(m02);
         index_data.push_back(m01);
         index_data.push_back(m12);
      }

      indices.clear();
      indices = index_data;
      index_data.clear();
   }
}