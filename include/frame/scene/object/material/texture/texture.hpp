#pragma once

#include <memory>
#include <GL/glew.h>

#include <utils/image.hpp>

namespace LE {
   class texture_t {
   public:
      texture_t(image_ptr_t const & image, GLenum texture_type = GL_TEXTURE_2D);

      void bind() const;
      void unbind() const;

   private:
      GLuint id_;
      GLenum texture_type_;
   };

   typedef std::shared_ptr<texture_t> texture_ptr_t;
}