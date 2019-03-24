#include <GL/glew.h>

#include <light_engine.hpp>

using namespace LE;

#ifndef VERSION
const std::string light_engine_t::version = "No version defined";
#else
const std::string light_engine_t::version = VERSION;
#endif

light_engine_t::light_engine_t() {
   glewInit();
}

void light_engine_t::resize(unsigned int width, unsigned int height) {
   unsigned int idx = 0;
   for (auto const & frame : frames_) {
      frame->resize(frames_poses_[idx][0].x * width, frames_poses_[idx][0].y * height, frames_poses_[idx][1].x * width, frames_poses_[idx][1].y * height);
      idx++;
   }
}

void light_engine_t::redraw() const {
   for (auto const & frame : frames_)
      frame->draw();
}

void light_engine_t::add_frame(frame_ptr_t const & frame, glm::vec2 const & pos, glm::vec2 const & size) {
   frames_.push_back(frame);
   frames_poses_.push_back({ pos, size });
}
