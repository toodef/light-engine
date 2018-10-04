#include <GL/glew.h>

#include <light_engine.hpp>

using namespace LE;

#ifndef VERSION
const std::string light_engine_t::version = "No version defined";
#elif
const std::string light_engine_t::version = VERSION;
#endif

light_engine_t::light_engine_t() {
   glewInit();
}

void light_engine_t::resize(unsigned int width, unsigned int height) {
   for (auto const & frame : frames_)
      frame->resize(width, height);
}

void light_engine_t::redraw() const {
   for (auto const & frame : frames_)
      frame->draw();
}

void light_engine_t::add_frame(frame_ptr_t const & frame) {
   frames_.push_back(frame);
}
