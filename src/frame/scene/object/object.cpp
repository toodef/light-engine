#include <GL/glew.h>

#include "../../../../include/frame/scene/object/object.hpp"

using namespace LE;

object_t::object_t(buffer_ptr_t const & buffer) : object_t(buffer, shader_prog_t::create_default()) {}

object_t::object_t(buffer_ptr_t const& buffer, material_ptr_t const & material) :
   buffer_(buffer), material_(material), drawing_style_(DS_points)
{}

GLenum convert_drawing_style(object_t::drawing_style_t drawing_style) {
   switch (drawing_style) {
   case object_t::DS_points:
      return GL_POINTS;
   case object_t::DS_triangles:
      return GL_TRIANGLES;
   case object_t::DS_lines:
      return GL_LINES;
   }
}

void object_t::draw() const {
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

   buffer_->bind();
   buffer_->enable_vertex_attribs();
   material_->bind();

   draw_buffer();

   material_->unbind();
   buffer_->disable_vertex_attribs();
   buffer_->unbind();
}

void object_t::draw_buffer() const {
   if (buffer_->have_idx_buffer())
      glDrawElements(convert_drawing_style(drawing_style_), buffer_->indices_number(), GL_UNSIGNED_INT, NULL);
   else
      glDrawArrays(convert_drawing_style(drawing_style_), 0, buffer_->vertices_number());
}

void object_t::set_drawing_style(object_t::drawing_style_t drawing_style) {
   drawing_style_ = drawing_style;
}

void object_t::set_uniforms_callback(set_uniforms_callback_t const & callback) {
   set_uniforms_callback_ = callback;
}

object_t::set_uniforms_callback_t object_t::set_uniforms_callback() {
   return set_uniforms_callback_;
}
