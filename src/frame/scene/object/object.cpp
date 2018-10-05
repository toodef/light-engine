#include <GL/glew.h>

#include "../../../../include/frame/scene/object/object.hpp"

using namespace LE;

object_t::object_t(buffer_ptr_t const & buffer, shader_prog_ptr_t const & shader_prog) : buffer_(buffer), shader_prog_(shader_prog) {}
object_t::object_t(buffer_ptr_t const & buffer) : object_t(buffer, shader_prog_t::create_default()) {}

void object_t::draw() const {
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

   buffer_->bind();
   buffer_->enable_vertex_attribs();

   shader_prog_->bind();
   if (set_uniforms_callback_)
      set_uniforms_callback_(shader_prog_);

   glDrawArrays(GL_POINTS, 0, buffer_->vertices_number());

   buffer_->disable_vertex_attribs();
   buffer_->unbind();

   shader_prog_->unbind();
}

void object_t::set_uniforms_callback(set_uniforms_callback_t const & callback) {
   set_uniforms_callback_ = callback;
}

