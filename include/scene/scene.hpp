#pragma once

#include <vector>

#include "../scene/object/object.hpp"

namespace LE {
   class scene_t : public std::vector<object_t> {
   public:
      void draw() const;
   };
}
