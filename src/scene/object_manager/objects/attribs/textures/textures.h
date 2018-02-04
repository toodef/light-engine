#pragma once

#include "../../../../../base.h"

namespace textures
{
   class texture_t;
   class texture_manager_t;

   enum texture_type_t
   {
        TT_none = 1
      , TT_2D
      , TT_cubemap
      , TT_3D
   };

   typedef shared_ptr<texture_t>            texture_ptr_t;
   typedef intrusive_ptr<texture_manager_t> texture_manager_ptr_t;

   struct texture_params
   {
      texture_params( texture_type_t type = TT_2D, unsigned int aniso_lvl = 0, bool generate_mipmap = false ) :
           type_           (type)
         , aniso_lvl_      (aniso_lvl)
         , generate_mipmap_(generate_mipmap)
         , mag_min_filters_(true)
      {
      }

      texture_type_t type_;

      unsigned int aniso_lvl_;

      bool   generate_mipmap_
           , mag_min_filters_;
   };

   typedef texture_params tex_params_t;

   class texture_manager_t
   {
   public:
      DECLARE_INTRUSIVE_COUNTER(texture_manager_t)

      texture_manager_t();
      ~texture_manager_t();

      texture_ptr_t create_texture( const string & file_name, tex_params_t const & params );
      texture_ptr_t create_texture( const string & file_name, GLenum type );
      texture_ptr_t create_texture( const vector<string> & files_name, tex_params_t const & params );
      texture_ptr_t create_texture( const vector<string> & files_name );

      void bind_texture( texture_t * texture = 0 );

      texture_t * bound_texture() const;

   private:
      texture_t * bound_texture_;
   };

   class texture_t
   {
   public:
      DECLARE_INTRUSIVE_COUNTER(texture_t)

      texture_t( string const & file_name, texture_manager_ptr_t const & manager
                ,tex_params_t const & params );
      texture_t( string const & file_name, texture_manager_ptr_t const & manager, GLenum type );

      texture_t( vector<string> const & files_name, texture_manager_ptr_t const & manager
                ,tex_params_t const & params );
      texture_t( vector<string> const & files_name, texture_manager_ptr_t const & manager );

      ~texture_t();

      GLuint gl_id() const;

      int unit () const;

      GLenum type() const;

   private:
      void load_2d      ( const string & file_name, tex_params_t const & params );
      void load_2d      ( const string & file_name );

      void load_cube_map( const string & file_name, tex_params_t const & params );
      void load_cube_map( const string & file_name );

      void load_cube_map( const vector<string> & files_names, tex_params_t const & params );
      void load_cube_map( const vector<string> & files_names );

   private:
      texture_manager_ptr_t manager_;

      GLenum type_;

      GLuint id_;
   };
}