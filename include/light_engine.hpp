#pragma once

#include <glm/vec3.hpp>
#include <vector>

#include "frame/frame.hpp"

namespace LE {
   class light_engine_t {
   public:
      light_engine_t();

      void resize(unsigned int width, unsigned int height);
      void redraw() const;

      void add_frame(frame_ptr_t const & frame);

      static const std::string version;

   private:
      std::vector<frame_ptr_t> frames_;
   };
}
