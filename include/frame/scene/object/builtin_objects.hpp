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
}
