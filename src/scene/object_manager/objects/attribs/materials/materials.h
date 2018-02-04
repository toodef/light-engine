#pragma once

#include "../../../../../base.h"

namespace materials
{
   using namespace glm;

   class material_manager_t;
   class material_t;
   class uniform_t;

   typedef boost::intrusive_ptr<material_manager_t> material_manager_ptr_t;
   typedef boost::shared_ptr<material_t>            material_ptr_t;
   typedef boost::weak_ptr<material_t>              material_weak_ptr_t;
   typedef boost::shared_ptr<uniform_t>             uniform_ptr_t;

////////////////////////////////////////
/// exceptions
////////////////////////////////////////
   struct material_exception_t : std::exception
   {
      material_exception_t()
      {
      }

      material_exception_t( const string & text ) : text_(text)
      {
      }

      virtual void print_exception()
      {
         cerr << text_ << endl;
      }

      const string text_;
   };

   struct create_shader_exception_t : material_exception_t
   {
      create_shader_exception_t()
      {
      }

      create_shader_exception_t( const string & text ) : text_(text)
      {
      }

      virtual void print_exception()
      {
         cerr << text_ << endl;
      }

      const string text_;
   };

   struct load_exception_t : create_shader_exception_t
   {
      load_exception_t( const string & text ) : text_(text)
      {
      }

      void print_exception()
      {
         cerr << text_ << endl;
      }

      const string text_;
   };

   struct compile_exception_t : create_shader_exception_t
   {
      compile_exception_t( const string & text, string const & err_list ) : text_(text),
                                                                          err_list_(err_list)
      {
      }

      void print_exception()
      {
         cerr << text_ << endl;
         cerr << "Error list:" << endl << err_list_ << endl;
      }

      const string text_;
      const string err_list_;
   };

   struct create_prog_exception_t : create_shader_exception_t
   {
      create_prog_exception_t( const string & text ) : text_(text)
      {
      }

      void print_exception()
      {
         cerr << text_ << endl;
      }

      const string text_;
   };

////////////////////////////////////////
/// material_manager class
////////////////////////////////////////
   class material_manager_t
   {
   public:
      DECLARE_INTRUSIVE_COUNTER(material_manager_t)

      material_manager_t();
      ~material_manager_t(); // Assert for zero bound material

      material_ptr_t create_material( const string & vertex_shader_file, const string & fragment_shader_file
                                     ,const string & geometry_shader_file = "" );

      material_ptr_t create_material_from_source( const string & vertex_shader, const string & fragment_shader
                                                 ,const string & geometry_shader = "" );

      void bind_material( material_t * material = 0 );

      material_t * bound_material() const;

   private:
      material_t * bound_material_;
   };

////////////////////////////////////////
/// uniform structure and class
////////////////////////////////////////
   struct uniform_info_t
   {
      uniform_info_t():
           name          (string())
         , id            (0)
         , buffer_id     (0)
         , index         (0)
         , binding_point (0)
         , type          (0)
         , init          (0)
      {
      }

      string name;
      GLint  id;
      GLuint buffer_id;
      GLuint index;
      GLuint binding_point;
      GLenum type;

      bool init;
   };

   class uniform_t
   {
   public:
      uniform_t ( material_ptr_t const & mat = material_ptr_t(), uniform_info_t const & info = uniform_info_t() );
      ~uniform_t();

      void set( vector<vec2> const & data );
      void set( vector<vec3> const & data );
      void set( vector<vec4> const & data );
      void set( vector<int> const & data );
      void set( vector<float> const & data );
      void set( int const * data          , size_t count = 1 );
      void set( unsigned int const * data , size_t count = 1 );
      void set( float const * data        , size_t count = 1 );
      void set( float const data );
      void set( int unit ); // for textures only
      void set( vec2 const & );
      void set( vec3 const & );
      void set( vec4 const & );
      void set( mat2 const & );
      void set( mat3 const & );
      void set( mat4 const * );

      bool init    () const;
      void set_init( bool is_init );

   private:
      material_manager_ptr_t manager_;
      material_ptr_t const   mat_;
      uniform_info_t         info_;
   };

////////////////////////////////////////
/// material_t class
////////////////////////////////////////
   class material_t
   {
   public:
      material_t( material_manager_ptr_t const & manager );

      ~material_t();

      uniform_ptr_t uniform( string const & name );

      GLuint gl_id() const;

      void load_material_from_source( string const & vertex_shader_src, string const & fragment_shader_src, string const & geometry_shader_src = "" );
      void load_material_from_file  ( string const & vertex_shader_file, string const & fragment_shader_file, string const & geometry_shader_file = "" );

      uniform_ptr_t & new_uniform( string const & name );

      string vs_file_name() const;
      string fs_file_name() const;
      string gs_file_name() const;

      void fill_uniforms_info( material_ptr_t const & material );

      bool is_init_;

   private:
      struct shader_t
      {
         shader_t( GLenum type ) :
               id_(0)
         {
            if (!(id_ = glCreateShader(type)))
               throw create_shader_exception_t("Error get shader id!");
         }

         GLuint id_;
      };

      typedef auto_ptr<shader_t> shader_auto_ptr_t;

      shader_auto_ptr_t create_shader( string const & source, GLenum type );

   private:
      typedef map<string, uniform_info_t> uniforms_info_t;
      typedef map<string, uniform_ptr_t>  uniforms_t;

      material_t();

      vector<string> uniforms_names_;

      material_manager_ptr_t manager_;
      uniforms_info_t        uniforms_info_;
      uniforms_t             uniforms_;
      shader_auto_ptr_t      vs_, fs_, gs_;

      material_weak_ptr_t this_ptr_;

      string   vertex_shader_file_
             , fragment_shader_file_
             , geometry_shader_file_;

      GLuint prog_;
   };
}
