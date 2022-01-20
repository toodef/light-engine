#define _USE_MATH_DEFINES
#include<cmath>

#include <frame/scene/object/builtin_objects.hpp>

using namespace LE;

sphere_t::sphere_t(glm::vec3 const& center, float radius, size_t detalisation) : center_(center), radius_(radius), detalisation_(detalisation)  {}

void sphere_t::set_shader_prog(shader_prog_ptr_t const & prog) { shader_prog_ = prog; }
void sphere_t::set_texture(texture_ptr_t const & texture) { texture_ = texture; }
void sphere_t::set_color(glm::vec3 const& color) { color_ = color; color_was_set_ = true; }

void sphere_t::generate_tex_coords() { generate_tex_coords_ = true; }
void sphere_t::generate_normales() { generate_normales_ = true; }

object_ptr_t sphere_t::compile() const {
   std::vector<glm::vec3> vertices;
   std::vector<unsigned int> indices;
   generate_vertices_(vertices, indices);

   buffer_ptr_t buffer;
   std::vector<glm::vec3> normales;
   std::vector<glm::vec2> tex_coords;

   if (generate_tex_coords_) {
      tex_coords.resize(vertices.size());
      for (unsigned int i = 0; i < vertices.size(); ++i)
         tex_coords[i] = glm::vec2(atan2f(vertices[i].x, vertices[i].z) / (2 * M_PI) + 0.5f, 0.5f - asinf(vertices[i].y) / M_PI);
   }

   if (generate_normales_) {
      normales.resize(vertices.size());
      for (unsigned int i = 0; i < vertices.size(); ++i)
         normales[i] = vertices[i];
   }

   for (unsigned int i = 0; i < vertices.size(); ++i)
      vertices[i] = vertices[i] * radius_ + center_;

   if (generate_tex_coords_) {
      if (color_was_set_)
         if (generate_normales_)
            buffer = std::make_shared<buffer_t>(vertices, normales, color_, tex_coords);
         else
            buffer = std::make_shared<buffer_t>(vertices, color_, tex_coords);
      else
         if (generate_normales_)
            buffer = std::make_shared<buffer_t>(vertices, normales, tex_coords);
         else
            buffer = std::make_shared<buffer_t>(vertices, tex_coords);
   }
   else {
      if (color_was_set_)
         if (generate_normales_)
            buffer = std::make_shared<buffer_t>(vertices, normales, color_);
         else
            buffer = std::make_shared<buffer_t>(vertices, color_);
      else
         if (generate_normales_)
            buffer = std::make_shared<buffer_t>(vertices, normales);
         else
            buffer = std::make_shared<buffer_t>(vertices);
   }

   buffer->add_index_buffer(indices);
   auto object = std::make_shared<object_t>(buffer, shader_prog_, std::vector<texture_ptr_t>{ texture_ });
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
      0 , 1 , 4 ,
      0 , 4 , 9 ,
      9 , 4 , 5 ,
      4 , 8 , 5 ,
      4 , 1 , 8 ,
      8 , 1 , 10,
      8 , 10, 3 ,
      5 , 8 , 3 ,
      5 , 3 , 2 ,
      2 , 3 , 7 ,
      7 , 3 , 10,
      7 , 10, 6 ,
      7 , 6 , 11,
      11, 6 , 0 ,
      0 , 6 , 1 ,
      6 , 10, 1 ,
      9 , 11, 0 ,
      9 , 2 , 11,
      9 , 5 , 2 ,
      7 , 11, 2};

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
         index_data.push_back(m02);
         index_data.push_back(m01);
         index_data.push_back(i1);
         index_data.push_back(m01);
         index_data.push_back(m12);
         index_data.push_back(i2);
         index_data.push_back(m12);
         index_data.push_back(m02);
         index_data.push_back(m02);
         index_data.push_back(m12);
         index_data.push_back(m01);
      }

      indices.clear();
      indices = index_data;
      index_data.clear();
   }
}