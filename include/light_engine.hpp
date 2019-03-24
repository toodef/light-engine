#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include <array>

#include "frame/frame.hpp"

namespace LE {
   class light_engine_t {
   public:
      light_engine_t();

      void resize(unsigned int width, unsigned int height);
      void redraw() const;

      void add_frame(frame_ptr_t const & frame, glm::vec2 const & pos = glm::vec2(0, 0), glm::vec2 const & size = glm::vec2(1, 1));
      frame_ptr_t get_frame(unsigned int idx) const;

      static const std::string version;

   private:
      std::vector<frame_ptr_t> frames_;
      std::vector<std::array<glm::vec2, 2>> frames_poses_;
   };
}
