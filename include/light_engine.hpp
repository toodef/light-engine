#pragma once

#include "scene/scene.hpp"

namespace LE {
   class light_engine_t {
      light_engine_t(unsigned int width, unsigned int height);

   private:
      std::vector<scene_t> scenes_;
   };
}
