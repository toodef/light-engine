#include "../../../../../base.h"

using namespace SE;

////////////////////////////////////////
// texture_t class methods
////////////////////////////////////////

texture_t::texture_t( texture_manager_ptr_t const & manager, tex_params_t const & params ) :
     manager_(manager)
   , type_(params.type_)
{
   switch(params.type_)
   {
   case GL_TEXTURE_2D:
      glEnable     (GL_TEXTURE_2D);
      glGenTextures(1, &id_);
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      glBindTexture(GL_TEXTURE_2D, id_);

      //load_2d(file_name, params);

      glBindTexture(GL_TEXTURE_2D, 0);
      glDisable    (GL_TEXTURE_2D);
      break;

   case GL_TEXTURE_CUBE_MAP:
      glEnable     (GL_TEXTURE_CUBE_MAP);
      glGenTextures(1, &id_);
      glBindTexture(GL_TEXTURE_CUBE_MAP, id_);

      //load_cube_map(file_name, params);

      glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
      glDisable    (GL_TEXTURE_CUBE_MAP);
      break;
   }
}

texture_t::texture_t( texture_manager_ptr_t const & manager, GLenum type ) :
     manager_(manager)
   , type_(type)
{
  switch(type_)
  {
  case GL_TEXTURE_2D:
     glEnable     (GL_TEXTURE_2D);
     glGenTextures(1, &id_);
     glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
     glBindTexture(GL_TEXTURE_2D, id_);

     //load_2d(file_name);

     glBindTexture(GL_TEXTURE_2D, 0);
     glDisable    (GL_TEXTURE_2D);
     break;

  case GL_TEXTURE_CUBE_MAP:
     glEnable     (GL_TEXTURE_CUBE_MAP);
     glGenTextures(1, &id_);
     glBindTexture(GL_TEXTURE_CUBE_MAP, id_);

     //load_cube_map(file_name);

     glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
     glDisable    (GL_TEXTURE_CUBE_MAP);
     break;
  }
}

texture_t::texture_t( texture_manager_ptr_t const & manager ) :
     manager_(manager)
   , type_(GL_TEXTURE_CUBE_MAP)
{
   glEnable     (GL_TEXTURE_CUBE_MAP);
   glGenTextures(1, &id_);
   glBindTexture(GL_TEXTURE_CUBE_MAP, id_);

   //load_cube_map(files_name);

   glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
   glDisable    (GL_TEXTURE_CUBE_MAP);
}

texture_t::~texture_t()
{
   assert(manager_->bound_texture() != this);

   glDeleteTextures(1, &id_);
}

GLuint texture_t::gl_id() const
{
   return id_;
}

GLenum texture_t::type() const
{
   return type_;
}

//void texture_t::load_2d( const string & file_name, tex_params_t const & params )
//{
//   load_2d(file_name);
//
//   if (params.aniso_lvl_)
//   {
//      float largest = 0;
//
//      glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &largest);
//
//      if (largest < params.aniso_lvl_)
//      {
//         cerr << "This hardware not supported anisotropic filtering level higher than" << largest << "x !!!" << endl;
//         glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, largest);
//      }
//      else
//         glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, params.aniso_lvl_);
//   }
//
//   if (params.generate_mipmap_)
//   {
//      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//      glGenerateMipmap(GL_TEXTURE_2D);
//   }
//   else
//      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//}
//
//void texture_t::load_2d( const string & file_name )
//{
//   Magick::Image img;
//
//   img.read(file_name);
//
//   glTexEnvf      (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//
//   int res_y = img.rows()
//      ,res_x = img.columns();
//
//   scoped_array<unsigned char> data(new unsigned char[res_x * res_y * 3]);
//
//   img.write   (0, 0, res_x, res_y, "RGB", CharPixel, data.get());
//
//   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16, res_x, res_y, 0, GL_RGB, GL_UNSIGNED_BYTE
//                ,data.get());
//
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//
//   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//}
//
//void texture_t::load_cube_map( const string & file_name, tex_params_t const & params )
//{
//   load_cube_map(file_name);
//
//   if (params.aniso_lvl_)
//   {
//      float largest = 0;
//
//      glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &largest);
//
//      if (largest < params.aniso_lvl_)
//      {
//         cerr << "This hardware not supported anisotropic filtering level higher than " << largest << "x !!!" << endl;
//         glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, largest);
//      }
//      else
//         glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, params.aniso_lvl_);
//   }
//
//   if (params.generate_mipmap_)
//   {
//      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//      glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
//   }
//   else
//      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//}
//
//void texture_t::load_cube_map( const string & file_name )
//{
//   Magick::Image img_base, img;
//
//   img_base.read(file_name);
//
//   int res_x = img_base.rows() / 3
//      ,res_y = img_base.columns() / 4;
//
//   scoped_array<unsigned char> data(new unsigned char[res_x * res_y * 3]);
//
//   img = img_base;
//
//   img.flop();
//   img.crop    (Geometry(res_x, res_y, res_x * 2, res_y));
//   img.write   (0, 0, res_x, res_y, "RGB", CharPixel, data.get());
//   glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB16, res_x, res_y, 0, GL_RGB
//                ,GL_UNSIGNED_BYTE, data.get());
//
//   img = img_base;
//
//   img.flop();
//   img.crop    (Geometry(res_x, res_y, 0, res_y));
//   img.write   (0, 0, res_x, res_y, "RGB", CharPixel, data.get());
//   glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB16, res_x, res_y, 0, GL_RGB
//                ,GL_UNSIGNED_BYTE, data.get());
//
//   img = img_base;
//
//   img.flip();
//   img.crop    (Geometry(res_x, res_y, res_x, 0));
//   img.write   (0, 0, res_x, res_y, "RGB", CharPixel, data.get());
//   glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB16, res_x, res_y, 0, GL_RGB
//                ,GL_UNSIGNED_BYTE, data.get());
//
//   img = img_base;
//
//   img.flip();
//   img.crop    (Geometry(res_x, res_y, res_x, res_y * 2));
//   img.write   (0, 0, res_x, res_y, "RGB", CharPixel, data.get());
//   glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB16, res_x, res_y, 0, GL_RGB
//                ,GL_UNSIGNED_BYTE, data.get());
//
//   img = img_base;
//
//   img.flop();
//   img.crop    (Geometry(res_x, res_y, res_x * 3, res_y));
//   img.write   (0, 0, res_x, res_y, "RGB", CharPixel, data.get());
//   glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB16, res_x, res_y, 0, GL_RGB
//                ,GL_UNSIGNED_BYTE, data.get());
//
//   img = img_base;
//
//   img.flop();
//   img.crop    (Geometry(res_x, res_y, res_x, res_y));
//   img.write   (0, 0, res_x, res_y, "RGB", CharPixel, data.get());
//   glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB16, res_x, res_y, 0, GL_RGB
//                ,GL_UNSIGNED_BYTE, data.get());
//
//   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
//}
//
//void texture_t::load_cube_map( vector<string> const & files_names, tex_params_t const & params )
//{
//   load_cube_map(files_names);
//
//   if (params.aniso_lvl_)
//   {
//      float largest = 0;
//
//      glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &largest);
//
//      if (largest < params.aniso_lvl_)
//      {
//         cerr << "This hardware not supported anisotropic filtering level higher than" << largest << "x !!!" << endl;
//         glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, largest);
//      }
//      else
//         glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, params.aniso_lvl_);
//   }
//
//   if (params.generate_mipmap_)
//   {
//      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//      glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
//   }
//   else
//      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//}
//
//void texture_t::load_cube_map( vector<string> const & files_names )
//{
//   Magick::Image img;
//
//   for (int i = 0; i < files_names.size(); ++i)
//   {
//      img.read(files_names[i]);
//
//      int res_x = img.rows()
//         ,res_y = img.columns();
//
//      scoped_array<unsigned char> data(new unsigned char[res_x * res_y * 3]);
//
//      if (i == 2 || i == 3)
//         img.flip();
//      else
//         img.flop();
//
//      img.write(0, 0, res_x, res_y, "RGB", CharPixel, data.get());
//
//      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//
//      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16, res_x, res_y, 0, GL_RGB
//                   ,GL_UNSIGNED_BYTE, data.get());
//   }
//
//   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
//}

////////////////////////////////////////
// texture_manager_t class methods
////////////////////////////////////////

texture_manager_t::texture_manager_t() :
   bound_texture_(NULL)
{
}

texture_manager_t::~texture_manager_t()
{
   bound_texture_ = NULL;
}

texture_ptr_t texture_manager_t::create_texture( tex_params_t const & params )
{
   texture_ptr_t new_texture = std::make_shared<texture_t>(shared_from_this(), params);

   return new_texture;
}

texture_ptr_t texture_manager_t::create_texture( GLenum type )
{
   texture_ptr_t new_texture = std::make_shared<texture_t>(shared_from_this(), type);

   return new_texture;
}

texture_ptr_t texture_manager_t::create_texture()
{
   texture_ptr_t new_texture = std::make_shared<texture_t>(shared_from_this());

   return new_texture;
}

void texture_manager_t::bind_texture( texture_t * texture )
{
   static GLenum type;

   if (texture)
   {
      glBindTexture(texture->type(), texture->gl_id());

      glActiveTexture(GL_TEXTURE0);

      type = texture->type();
   }
   else
      glBindTexture(type, 0);

   if (glGetError())
      cerr << "Bind texture fail!" << endl;

   bound_texture_ = texture;
}

texture_t * texture_manager_t::bound_texture() const
{
   return bound_texture_;
}
