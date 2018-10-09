#include <memory>

#include <frame/scene/object/builtin_objects.hpp>

using namespace LE;

object_ptr_t builtin_objects_t::point(glm::vec3 const & position, glm::vec3 const & color, shader_prog_ptr_t const & shader_prog) {
   buffer_ptr_t buffer = std::make_shared<buffer_t>(std::vector<glm::vec3>{position}, color);
   return shader_prog ? std::make_shared<object_t>(buffer, shader_prog) : std::make_shared<object_t>(buffer);
}

object_ptr_t builtin_objects_t::triangle(std::vector<glm::vec3> const & vertices, glm::vec3 const & color, shader_prog_ptr_t const & shader_prog) {
   buffer_ptr_t buffer = std::make_shared<buffer_t>(vertices, color);
   auto object = shader_prog ? std::make_shared<object_t>(buffer, shader_prog) : std::make_shared<object_t>(buffer);
   object->set_drawing_style(object_t::DS_triangles);
   return object;
}

object_ptr_t builtin_objects_t::sphere(glm::vec3 const & center, float radius, glm::vec3 const & color, unsigned int detalisation, shader_prog_ptr_t const & shader_prog) {
   std::vector<glm::vec3> vertices(12);
   static const std::vector<unsigned int> indices {
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

   vertices[0] = center + glm::normalize(glm::vec3(-x, 0, z)) * radius;
   vertices[1] = center + glm::normalize(glm::vec3(x, 0, z)) * radius;
   vertices[2] = center + glm::normalize(glm::vec3(-x, 0, -z)) * radius;
   vertices[3] = center + glm::normalize(glm::vec3(x, 0, -z)) * radius;
   vertices[4] = center + glm::normalize(glm::vec3(0, z, x)) * radius;
   vertices[5] = center + glm::normalize(glm::vec3(0, z, -x)) * radius;
   vertices[6] = center + glm::normalize(glm::vec3(0, -z, x)) * radius;
   vertices[7] = center + glm::normalize(glm::vec3(0, -z, -x)) * radius;
   vertices[8] = center + glm::normalize(glm::vec3(z, x, 0)) * radius;
   vertices[9] = center + glm::normalize(glm::vec3(-z, x, 0)) * radius;
   vertices[10] = center + glm::normalize(glm::vec3(z, -x, 0)) * radius;
   vertices[11] = center + glm::normalize(glm::vec3(-z, -x, 0)) * radius;

   buffer_ptr_t buffer = std::make_shared<buffer_t>(vertices, color);
   buffer->add_index_buffer(indices);
   auto object = shader_prog ? std::make_shared<object_t>(buffer, shader_prog) : std::make_shared<object_t>(buffer);
   object->set_drawing_style(object_t::DS_triangles);
   return object;
}

object_ptr_t builtin_objects_t::quad(std::vector<glm::vec3> const & vertices, glm::vec3 const & color, shader_prog_ptr_t const & shader_prog) {
   static const std::vector<unsigned int> indices{
      0 , 1 , 2 ,
      0 , 2 , 3 };

   buffer_ptr_t buffer = std::make_shared<buffer_t>(vertices, color);
   buffer->add_index_buffer(indices);
   auto object = shader_prog ? std::make_shared<object_t>(buffer, shader_prog) : std::make_shared<object_t>(buffer);
   object->set_drawing_style(object_t::DS_triangles);
   return object;
}

object_ptr_t builtin_objects_t::box(glm::vec3 const & center, glm::vec3 const & up, glm::vec3 const & right, glm::vec3 const & forward, glm::vec3 const & color, shader_prog_ptr_t const & shader_prog) {
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
   auto object = shader_prog ? std::make_shared<object_t>(buffer, shader_prog) : std::make_shared<object_t>(buffer);
   object->set_drawing_style(object_t::DS_triangles);
   return object;
}

object_ptr_t builtin_objects_t::line(glm::vec3 const & v0, glm::vec3 const & v1, glm::vec3 const & color, shader_prog_ptr_t const & shader_prog) {
   const std::vector<glm::vec3> vertices = {v0, v1};
   buffer_ptr_t buffer = std::make_shared<buffer_t>(vertices, color);
   auto object = shader_prog ? std::make_shared<object_t>(buffer, shader_prog) : std::make_shared<object_t>(buffer);
   object->set_drawing_style(object_t::DS_lines);
   return object;
}
