#include <GL/glew.h>

#include <frame/scene/object/material/texture/texture.hpp>

using namespace LE;

texture_t::texture_t(image_ptr_t const & image, GLenum texture_type): texture_type_(texture_type) {
   glGenTextures(1, &id_);
   bind();
   glTexImage2D(texture_type, 0, GL_RGB, image->width(), image->height(), 0, GL_RGB, GL_UNSIGNED_BYTE, image->data());
   glGenerateMipmap(GL_TEXTURE_2D);
   unbind();
}

void texture_t::bind() const { glBindTexture(texture_type_, id_); }
void texture_t::unbind() const { glBindTexture(GL_TEXTURE_2D, 0); };
