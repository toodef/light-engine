#pragma once

#include <memory>
#include <functional>

#include <frame/scene/object/buffer/buffer.hpp>
#include <frame/scene/object/texture/texture.hpp>
#include <frame/scene/object/shader/shader.hpp>

namespace LE {
   class object_t;
   typedef std::shared_ptr<object_t> object_ptr_t;

   class object_t {
   public:
      enum drawing_style_t { DS_points, DS_line_strip, DS_line_loop, DS_lines, DS_line_strip_adjacency, DS_lines_adjacency, DS_triangle_strip, DS_trianle_fan, DS_triangles, DS_triangle_strip_adjacency, DS_triangles_adjacency, DS_patches };
      enum polygon_mode_t { PM_point, PM_line, PM_fill};

   public:
      object_t(buffer_ptr_t const & buffer);
      object_t(buffer_ptr_t const & buffer, shader_prog_ptr_t const& shader_prog = 0,
               std::vector<texture_ptr_t> const & texture = std::vector<texture_ptr_t>());

      void draw() const;

      void set_drawing_style(drawing_style_t drawing_style);
      void set_polygon_mode(polygon_mode_t polygon_mode);
      void set_points_size(float point_size);
      void set_lines_width(float lines_width);
      void enable_face_culling(bool cull_front, bool cull_back);

      typedef std::function<void(shader_prog_ptr_t const &)> set_uniforms_callback_t;
      void set_uniforms_callback(set_uniforms_callback_t const & callback);
      set_uniforms_callback_t set_uniforms_callback();

      static std::vector<object_ptr_t> load_obj(std::string const & path);

   protected:
      virtual inline void draw_buffer() const;

      buffer_ptr_t buffer_;
      std::vector<texture_ptr_t> textures_;
      shader_prog_ptr_t shader_prog_;

      set_uniforms_callback_t set_uniforms_callback_;

      GLenum drawing_style_, polygon_mode_;

      float point_size_ = 1.f, lines_width_ = 1.f;

      bool cull_faces_ = true;
      GLenum cull_faces_mode_ = GL_BACK;
   };
}
