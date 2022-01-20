#include <GL/glew.h>
#include <filesystem>
#include <map>

#include <frame/scene/object/object.hpp>

using namespace LE;

object_t::object_t(buffer_ptr_t const & buffer) : object_t(buffer, shader_prog_t::create_default()) {}

object_t::object_t(buffer_ptr_t const& buffer, shader_prog_ptr_t const& shader_prog, std::vector<texture_ptr_t> const& textures) :
   buffer_(buffer), shader_prog_(shader_prog), textures_(textures), drawing_style_(GL_POINTS), polygon_mode_(GL_FILL)
{}

void object_t::draw() const {
   glPolygonMode(GL_FRONT_AND_BACK, polygon_mode_);

   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_BLEND);

   if (cull_faces_) {
      glEnable(GL_CULL_FACE);
      glCullFace(cull_faces_mode_);
   }

   buffer_->bind();
   buffer_->enable_vertex_attribs();

   if (shader_prog_)
      shader_prog_->bind();
   size_t texture_id = 0;
   for (texture_ptr_t const& texture : textures_)
      if (texture) {
         glActiveTexture(GL_TEXTURE0 + texture_id);
         texture->bind();
         texture_id++;
      }

   if (shader_prog_ && set_uniforms_callback_)
      set_uniforms_callback_(shader_prog_);

   glPointSize(point_size_);
   glLineWidth((GLfloat)lines_width_);
   draw_buffer();

   for (texture_ptr_t const& texture : textures_)
      if (texture)
         texture->unbind();
   if (shader_prog_)
      shader_prog_->unbind();

   buffer_->disable_vertex_attribs();
   buffer_->unbind();

   if (cull_faces_)
      glDisable(GL_CULL_FACE);
}

void object_t::draw_buffer() const {
   if (buffer_->have_idx_buffer())
      glDrawElements(drawing_style_, buffer_->indices_number(), GL_UNSIGNED_INT, NULL);
   else
      glDrawArrays(drawing_style_, 0, buffer_->vertices_number());
}

void object_t::set_drawing_style(object_t::drawing_style_t drawing_style) {
   switch (drawing_style) {
   case DS_points:
      drawing_style_ = GL_POINTS;
      break;
   case DS_line_strip:
      drawing_style_ = GL_LINE_STRIP;
      break;
   case DS_line_loop:
      drawing_style_ = GL_LINE_LOOP;
      break;
   case DS_lines:
      drawing_style_ = GL_LINES;
      break;
   case DS_line_strip_adjacency:
      drawing_style_ = GL_LINE_STRIP_ADJACENCY;
      break;
   case DS_lines_adjacency:
      drawing_style_ = GL_LINES_ADJACENCY;
      break;
   case DS_triangle_strip:
      drawing_style_ = GL_TRIANGLE_STRIP;
      break;
   case DS_trianle_fan:
      drawing_style_ = GL_TRIANGLE_FAN;
      break;
   case DS_triangles:
      drawing_style_ = GL_TRIANGLES;
      break;
   case DS_triangle_strip_adjacency:
      drawing_style_ = GL_TRIANGLE_STRIP_ADJACENCY;
      break;
   case DS_triangles_adjacency:
      drawing_style_ = GL_TRIANGLES_ADJACENCY;
      break;
   case DS_patches:
      drawing_style_ = GL_PATCHES;
      break;
   }
}

void object_t::set_polygon_mode(polygon_mode_t polygon_mode) {
   switch (polygon_mode) {
   case PM_point:
      polygon_mode_ = GL_POINT;
      break;
   case PM_line:
      polygon_mode_ = GL_LINE;
      break;
   case PM_fill:
      polygon_mode_ = GL_FILL;
      break;
   }
}

void object_t::set_points_size(float point_size) { point_size_ = point_size; }
void object_t::set_lines_width(float lines_width) { lines_width_ = lines_width; }

void object_t::enable_face_culling(bool cull_front, bool cull_back) {
   cull_faces_ = true;
   if (cull_front && cull_back)
      cull_faces_mode_ = GL_FRONT_AND_BACK;
   else if (cull_back)
      cull_faces_mode_ = GL_BACK;
   else if (cull_front)
      cull_faces_mode_ = GL_FRONT;
   else
      cull_faces_ = false;
}

void object_t::set_uniforms_callback(set_uniforms_callback_t const & callback) {
   set_uniforms_callback_ = callback;
}

object_t::set_uniforms_callback_t object_t::set_uniforms_callback() {
   return set_uniforms_callback_;
}
