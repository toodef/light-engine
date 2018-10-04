#include <GL/glew.h>

#include "../../include/frame/frame.hpp"

using namespace LE;

frame_t::frame_t() : depth_test_enabled_(true) {}

void frame_t::add_scene(scene_ptr_t const & scene){
   scenes_.push_back(scene);
}

void frame_t::set_background_color(glm::vec3 const & color) {
   glClearColor(color[0], color[1], color[2], 1);
}

// TODO: make min coordinates non-zero
void frame_t::resize(unsigned int width, unsigned int height) {
   glViewport(0, 0, width, height);

   for (auto const & scene : scenes_) {
      scene->get_camera()->width(width);
      scene->get_camera()->height(height);
   }
}

void frame_t::enable_depth_test(bool is_enable) { depth_test_enabled_ = is_enable; };

void frame_t::draw() {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glDepthFunc(GL_LEQUAL);
   
   if (depth_test_enabled_)
      glEnable(GL_DEPTH_TEST);

   for (auto const & scene : scenes_)
      scene->draw();

   if (depth_test_enabled_)
      glDisable(GL_DEPTH_TEST);
}