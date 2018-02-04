#pragma once

#include "../../../../../base.h"

namespace buffers
{
   class buffer_t;
   class buffer_manager_t;

   using namespace materials;

   typedef boost::shared_ptr<buffer_t>            buffer_ptr_t        ;
   typedef boost::intrusive_ptr<buffer_manager_t> buffer_manager_ptr_t;
   typedef vector<buffer_ptr_t>                   buffers_t           ;

   struct vertex_t
   {
      vertex_t( vec3 vertex = vec3(), vec3 normal = vec3(), vec3 tex_coord = vec3(), vec4 color = vec4() ) :
            vertex_   (vertex)
            , normal_   (normal)
            , tex_coord_(tex_coord)
            , color_    (color)
      {}

      vec3 vertex_;
      vec3 normal_;
      vec3 tex_coord_;
      vec4 color_;
   };

   typedef shared_ptr<vertex_t> vertex_ptr_t ;
   typedef vector<vertex_t>     vertices_t   ;

   class buffer_t
   {
   public:
      buffer_t ( const buffer_manager_ptr_t & manager );
      ~buffer_t();

      void data( vertices_t data, GLenum usage_type = GL_STATIC_DRAW );

      void * map( GLenum access_type = GL_WRITE_ONLY );
      void unmap();

      vertices_t vertices() const;

      GLuint id() const;

      size_t size() const;

   private:
      GLuint id_;

      buffer_manager_ptr_t manager_;

      vertices_t vertices_;
   };

   class buffer_manager_t
   {
   public:
      DECLARE_INTRUSIVE_COUNTER(buffer_manager_t)

      buffer_manager_t ();
      ~buffer_manager_t();

      buffer_ptr_t create_buffer( vertices_t data = vertices_t(), GLenum usage_type = GL_STATIC_DRAW );

      void bind_buffer( buffer_t * buffer = 0, GLenum buffer_target = GL_ARRAY_BUFFER );

      buffer_t * bound_buffer() const;

      // transform feedback functions
      static void set_tf_varyings( material_ptr_t const & material, const buffer_ptr_t & buffer
                                  ,const char ** names, GLsizei count = 1 );
      void * calculate_tf_data   ( const buffer_ptr_t & bounding_box_buffer, const buffer_ptr_t & tf_buffer );

   private:
      buffer_t * bound_buffer_;
   };
}