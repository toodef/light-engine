#include <GL/glew.h>

#include "../../include/frame/frame.hpp"

using namespace LE;

frame_t::frame_t() :
     depth_test_enabled_(true)
   , background_color_(0, 0, 0)
{}

void frame_t::add_scene(scene_ptr_t const & scene){
   scenes_.push_back(scene);
}

void frame_t::set_background_color(glm::vec3 const & color) {
   background_color_ = color;
}

// TODO: make min coordinates non-zero
void frame_t::resize(unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
   x_ = x; y_ = y; height_ = height; width_ = width;

   for (auto const & scene : scenes_) {
      scene->get_camera()->width(width);
      scene->get_camera()->height(height);
   }
}

void frame_t::enable_depth_test(bool is_enable) { depth_test_enabled_ = is_enable; };

void frame_t::draw() {
   glViewport(x_, y_, width_, height_);
   glEnable(GL_SCISSOR_TEST);
   glScissor(x_, y_, width_, height_);
   glClearColor(background_color_.x, background_color_.y, background_color_.z, 1);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glDisable(GL_SCISSOR_TEST);
   glDepthFunc(GL_LEQUAL);
   
   if (depth_test_enabled_)
      glEnable(GL_DEPTH_TEST);

   for (auto const & scene : scenes_)
      scene->draw();

   if (depth_test_enabled_)
      glDisable(GL_DEPTH_TEST);
}