#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/vec3.hpp>
#include <vector>

#include "object.hpp"

namespace LE {
   class builtin_objects_t {
   public:
      static object_ptr_t point(glm::vec3 const & position, glm::vec3 const & color, shader_prog_ptr_t const & shader_prog = 0);
      static object_ptr_t triangle(std::vector<glm::vec3> const & vertices, glm::vec3 const & color, shader_prog_ptr_t const & shader_prog = 0);
      static object_ptr_t sphere(glm::vec3 const & center, float radius, glm::vec3 const & color, unsigned int detalisation, shader_prog_ptr_t const & shader_prog = 0);
      static object_ptr_t quad(std::vector<glm::vec3> const & vertices, glm::vec3 const & color, shader_prog_ptr_t const & shader_prog = 0);
      static object_ptr_t box(glm::vec3 const & center, glm::vec3 const & up, glm::vec3 const & right, glm::vec3 const & forward, glm::vec3 const & color, shader_prog_ptr_t const & shader_prog = 0);
      static object_ptr_t point_cloud(std::vector<glm::vec3> const & positions, glm::vec3 const & color, shader_prog_ptr_t const & shader_prog = 0);
      static object_ptr_t line(glm::vec3 const & v0, glm::vec3 const & v1, glm::vec3 const & color, shader_prog_ptr_t const & shader_prog = 0);
   };

   namespace BuiltinObjects{
      struct builtin_objects_exception_t: std::exception {
         builtin_objects_exception_t(std::string const & message);
         virtual char const * what() const noexcept;

      private:
         std::string msg_;
      };

      class point_cloud_t {
      public:
         point_cloud_t(std::vector<glm::vec3> const & positions);

         void set_color(glm::vec3 const & color);
         void set_colors(std::vector<glm::vec3> const & colors);
         void set_normales(std::vector<glm::vec3> const & normales);
         void set_shader_prog(shader_prog_ptr_t const & shader_prog);

         object_ptr_t compile();

      private:
         std::vector<glm::vec3> pos_, colors_, normales_;
         shader_prog_ptr_t shader_prog_;
      };
   }
}
