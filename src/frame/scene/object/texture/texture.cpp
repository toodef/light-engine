#include <GL/glew.h>

#include <frame/scene/object/texture/texture.hpp>

using namespace LE;

texture_t::texture_t(image_ptr_t const & image, GLenum texture_type): texture_type_(texture_type) {
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
   glGenTextures(1, &id_);
   bind();

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

   GLenum format = GL_RGB;
   if (image->channels_num() == 1)
      format = GL_R;
   else if (image->channels_num() == 4)
      format = GL_RGBA;
   
   glTexImage2D(texture_type, 0, format, image->width(), image->height(), 0, format, GL_UNSIGNED_BYTE, image->data().get());
   
   //glGenerateMipmap(GL_TEXTURE_2D);
   unbind();
}

void texture_t::bind() const { glBindTexture(texture_type_, id_); }
void texture_t::unbind() const { glBindTexture(GL_TEXTURE_2D, 0); };
