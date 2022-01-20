#pragma once

#include <memory>
#include <functional>

#include <frame/scene/object/buffer/buffer.hpp>
#include <frame/scene/object/material/material.hpp>

namespace LE {
   class object_t {
   public:
      enum drawing_style_t { DS_points, DS_triangles, DS_lines };

   public:
      object_t(buffer_ptr_t const & buffer);
      object_t(buffer_ptr_t const & buffer, material_ptr_t const & material);

      void draw() const;

      void set_drawing_style(drawing_style_t drawing_style);

   protected:
      virtual inline void draw_buffer() const;

      buffer_ptr_t buffer_;
      material_ptr_t material_;

      drawing_style_t drawing_style_;
   };

   typedef std::shared_ptr<object_t> object_ptr_t;
}
