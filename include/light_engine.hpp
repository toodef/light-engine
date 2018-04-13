#pragma once

#include <glm/vec3.hpp>

#include "scene/scene.hpp"

namespace LE {
   class light_engine_t {
   public:
      light_engine_t();

      void resize(unsigned int height, unsigned int width);

      void draw() const;

      void set_background_color(glm::vec3 const & color);

   private:
      std::vector<scene_t> scenes_;
   };
}
