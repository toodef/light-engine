#pragma once

#include <memory>
#include <functional>

#include "buffer/buffer.hpp"
#include "texture/texture.hpp"
#include "shader/shader.hpp"

namespace LE {
   class object_t {
   public:
      enum drawing_style_t { DS_points, DS_triangles, DS_lines };

   public:
      object_t(buffer_ptr_t const & buffer);
      object_t(buffer_ptr_t const & buffer, shader_prog_ptr_t const & shader_prog);

      void draw() const;

      void set_drawing_style(drawing_style_t drawing_style);

      typedef std::function<void(shader_prog_ptr_t const &)> set_uniforms_callback_t;
      void set_uniforms_callback(set_uniforms_callback_t const & callback);
      set_uniforms_callback_t set_uniforms_callback();

   protected:
      virtual inline void draw_buffer() const;

      buffer_ptr_t buffer_;
      texture_ptr_t texture_;
      shader_prog_ptr_t shader_prog_;

      set_uniforms_callback_t set_uniforms_callback_;

      drawing_style_t drawing_style_;
   };

   typedef std::shared_ptr<object_t> object_ptr_t;
}
