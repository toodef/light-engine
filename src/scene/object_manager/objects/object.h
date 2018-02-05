#pragma once

#include "../../../base.h"

namespace object
{
   using namespace materials;
   using namespace textures;
   using namespace buffers;
   using namespace render;
   using namespace environment;

   typedef vector<size_t>       idx_buffer_t ;
   typedef vector<idx_buffer_t> idx_buffers_t;

   class object_t;

   typedef std::shared_ptr<object_t> obj_ptr_t;

   class object_t
   {
   public:
      object_t( effect_t const & effect, buffers_t const & buffers
               ,render_param_t const & render_param, vec3 pos = vec3()
               ,vec4 color = vec4(1.0f), float size = 1.0f
               ,idx_buffers_t const & idx_buffers = idx_buffers_t() );

      ~object_t();

      material_ptr_t get_material() const;
      texture_ptr_t  get_texture () const;
      buffer_ptr_t   get_buffer  ( size_t idx = 0 ) const;

      virtual void draw( env_ptr_t const & env );

      GLenum get_draw_style() const;

      render_param_t get_render_param() const;

      virtual void set_wireframe( bool is_wireframe );

      virtual void set_effect( effect_t const & effect );

      virtual void set_color( vec4 const & color );
      virtual vec4 color    () const;

      virtual void is_draw( bool need_draw );
      virtual bool is_draw();

      virtual vec3  pos () const;
      virtual float size() const;

      virtual void set_failed_plane_idx( size_t idx );
      virtual size_t failed_plane_idx  () const;

   protected:
      material_ptr_t material_   ;
      texture_ptr_t  texture_    ;
      buffers_t      buffers_    ;
      idx_buffers_t  idx_buffers_;

      render_param_t render_param_;

      size_t effect_id_;

      bool need_draw_;

      vec3 pos_;

      vec4 color_;

      float size_;

      GLenum drawing_style_;

      size_t last_failed_idx_;

      vector<bool>   f_plane_indexes_
                   , f_plane_is_checked_;
   };
}