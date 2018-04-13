#pragma once

#include <memory>

#include "../../scene/object/texture/texture.hpp"
#include "../../scene/object/shader/shader.hpp"

namespace LE {
   class object_t {
   public:
      void draw();

   private:
      texture_ptr_t texture_;
      shader_ptr_t shader_;
   };

   typedef std::shared_ptr<object_t> object_ptr_t;
}
