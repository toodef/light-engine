#pragma once

#include <glm/vec3.hpp>

#include "object.hpp"

namespace LE {
   class builtin_objects_t {
   public:
      static object_ptr_t point(glm::vec3 const & position, glm::vec3 const & color, shader_prog_ptr_t const & shader_prog = 0);
      object_ptr_t triangle();
      object_ptr_t sphere();
      object_ptr_t plane();
      object_ptr_t qube();
   };
}
