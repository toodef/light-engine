#include <memory>

#include <frame/scene/object/builtin_objects.hpp>

using namespace LE;

object_ptr_t builtin_objects_t::point(glm::vec3 const & position, glm::vec3 const & color, shader_prog_ptr_t const & shader_prog, texture_ptr_t const& texture) {
   buffer_ptr_t buffer = std::make_shared<buffer_t>(std::vector<glm::vec3>{position}, color);
   return std::make_shared<object_t>(buffer, shader_prog, texture);
}

object_ptr_t builtin_objects_t::triangle(std::vector<glm::vec3> const & vertices, glm::vec3 const & color, shader_prog_ptr_t const & shader_prog, texture_ptr_t const& texture) {
   buffer_ptr_t buffer = std::make_shared<buffer_t>(vertices, color);
   auto object = std::make_shared<object_t>(buffer, shader_prog, texture);
   object->set_drawing_style(object_t::DS_triangles);
   return object;
}

object_ptr_t builtin_objects_t::sphere(glm::vec3 const & center, float radius, glm::vec3 const & color, unsigned int detalisation, shader_prog_ptr_t const & shader_prog,
   texture_ptr_t const& texture) {
   std::vector<glm::vec3> vertices(12);
   std::vector<unsigned int> index_data;
   std::vector<unsigned int> end_index_data{
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

   float x = 0.525731112119133606f;
   float z = 0.850650808352039932f;

   vertices[0] = glm::normalize(glm::vec3(-x, 0, z));
   vertices[1] = glm::normalize(glm::vec3(x, 0, z));
   vertices[2] = glm::normalize(glm::vec3(-x, 0, -z));
   vertices[3] = glm::normalize(glm::vec3(x, 0, -z));
   vertices[4] = glm::normalize(glm::vec3(0, z, x));
   vertices[5] = glm::normalize(glm::vec3(0, z, -x));
   vertices[6] = glm::normalize(glm::vec3(0, -z, x));
   vertices[7] = glm::normalize(glm::vec3(0, -z, -x));
   vertices[8] = glm::normalize(glm::vec3(z, x, 0));
   vertices[9] = glm::normalize(glm::vec3(-z, x, 0));
   vertices[10] = glm::normalize(glm::vec3(z, -x, 0));
   vertices[11] = glm::normalize(glm::vec3(-z, -x, 0));

   for (size_t n = 0; n < detalisation - 1; ++n)
   {
      for (size_t i = 0; i < end_index_data.size() - 2; i += 3)
      {
         size_t i0 = end_index_data[i];
         size_t i1 = end_index_data[i + 1];
         size_t i2 = end_index_data[i + 2];

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

      end_index_data.clear();
      end_index_data = index_data;
      index_data.clear();
   }

   for (unsigned int i = 0; i < vertices.size(); ++i)
      vertices[i] = vertices[i] * radius + center;

   buffer_ptr_t buffer = std::make_shared<buffer_t>(vertices, color);
   buffer->add_index_buffer(end_index_data);
   auto object = std::make_shared<object_t>(buffer, shader_prog, texture);
   object->set_drawing_style(object_t::DS_triangles);
   return object;
}

object_ptr_t builtin_objects_t::quad(std::vector<glm::vec3> const & vertices, glm::vec3 const & color, shader_prog_ptr_t const & shader_prog, texture_ptr_t const& texture) {
   static const std::vector<unsigned int> indices{
      0 , 1 , 2 ,
      0 , 2 , 3 };

   std::vector<glm::vec2> tex_coords = { glm::vec2(1.f, 1.f), glm::vec2(1.f, 0.f), glm::vec2(0.f, 0.f), glm::vec2(0.f, 1.f) };

   buffer_ptr_t buffer = std::make_shared<buffer_t>(vertices, color, tex_coords);
   buffer->add_index_buffer(indices);
   auto object = std::make_shared<object_t>(buffer, shader_prog, texture);
   object->set_drawing_style(object_t::DS_triangles);
   return object;
}

object_ptr_t builtin_objects_t::box(glm::vec3 const & center, glm::vec3 const & up, glm::vec3 const & right, glm::vec3 const & forward, glm::vec3 const & color,
   shader_prog_ptr_t const & shader_prog, texture_ptr_t const& texture) {
   const std::vector<glm::vec3> vertices = {
      center - up - right - forward,
      center + up - right - forward,
      center + up + right - forward,
      center - up + right - forward,
      center - up - right + forward,
      center + up - right + forward,
      center + up + right + forward,
      center - up + right + forward
   };

   static const std::vector<unsigned int> indices{
      0, 1, 2, 0, 2, 3,
      0, 1, 5, 0, 5, 4,
      4, 5, 6, 4, 6, 7,
      3, 2, 6, 3, 6, 7,
      1, 5, 6, 1, 6, 2,
      4, 0, 3, 4, 3, 7
   };

   buffer_ptr_t buffer = std::make_shared<buffer_t>(vertices, color);
   buffer->add_index_buffer(indices);
   auto object = std::make_shared<object_t>(buffer, shader_prog, texture);
   object->set_drawing_style(object_t::DS_triangles);
   return object;
}

object_ptr_t builtin_objects_t::point_cloud(std::vector<glm::vec3> const & positions, glm::vec3 const & color, shader_prog_ptr_t const & shader_prog, texture_ptr_t const& texture) {
   buffer_ptr_t buffer = std::make_shared<buffer_t>(positions, color);
   auto object = std::make_shared<object_t>(buffer, shader_prog, texture);
   object->set_drawing_style(object_t::DS_points);
   return object;
}

object_ptr_t builtin_objects_t::line(glm::vec3 const & v0, glm::vec3 const & v1, glm::vec3 const & color, shader_prog_ptr_t const & shader_prog, texture_ptr_t const& texture) {
   const std::vector<glm::vec3> vertices = {v0, v1};
   buffer_ptr_t buffer = std::make_shared<buffer_t>(vertices, color);
   auto object = std::make_shared<object_t>(buffer, shader_prog, texture);
   object->set_drawing_style(object_t::DS_lines);
   return object;
}

using namespace BuiltinObjects;

builtin_objects_exception_t::builtin_objects_exception_t(std::string const & message): msg_(message) {}
char const * builtin_objects_exception_t::what() const noexcept { return msg_.c_str(); }

point_cloud_t::point_cloud_t(std::vector<glm::vec3> const & positions): pos_(positions) {}

void point_cloud_t::set_color(glm::vec3 const & color) {
   colors_.resize(pos_.size());
   for (unsigned int i = 0; i < pos_.size(); ++i)
      colors_[i] = color;
}

void point_cloud_t::set_colors(std::vector<glm::vec3> const & colors) {
   if (colors.size() != pos_.size())
      throw builtin_objects_exception_t("Colors vector size doesn't equal to positions number");

   colors_.resize(pos_.size());
   for (unsigned int i = 0; i < pos_.size(); ++i)
      colors_[i] = colors[i];
}

void point_cloud_t::set_normales(std::vector<glm::vec3> const & normales) {
   if (normales.size() != pos_.size())
      throw builtin_objects_exception_t("Normales vector size doesn't equal to positions number");

   normales_.resize(pos_.size());
   for (unsigned int i = 0; i < pos_.size(); ++i)
      normales_[i] = normales[i];
}

void point_cloud_t::set_shader_prog(shader_prog_ptr_t const & shader_prog) {
   shader_prog_ = shader_prog;
}

object_ptr_t point_cloud_t::compile() {
   buffer_ptr_t buffer;
   if (normales_.size() > 0)
      buffer = std::make_shared<buffer_t>(pos_, normales_, colors_);
   else
      buffer = std::make_shared<buffer_t>(pos_, colors_);

   auto object = std::make_shared<object_t>(buffer, shader_prog_, nullptr);
   object->set_drawing_style(object_t::DS_points);
   return object;
}