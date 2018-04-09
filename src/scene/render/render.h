#pragma once

#include "../../base.h"

namespace render
{
   using namespace materials;
   using namespace textures;

   struct effect_t {
      material_ptr_t material;
      texture_ptr_t texture;
      size_t id;

      effect_t(material_ptr_t const & material, texture_ptr_t const & texture, size_t id):
         material(material),
         texture(texture),
         id(id)
      {}
   };
   
   typedef std::vector<effect_t> effects_t;

   // primitive render style
   enum render_style_t
   {
        DS_point          = 0x0000
      , DS_lines          = 0x0001
      , DS_line_loop      = 0x0002
      , DS_line_strip     = 0x0003
      , DS_triangles      = 0x0004
      , DS_triangle_strip = 0x0005
      , DS_triangle_fan   = 0x0006
      , DS_quads          = 0x0007
      , DS_quad_strip     = 0x0008
      , DS_polygon        = 0x0009
   };

   struct render_param_t
   {
      render_param_t() :
           is_wireframe   (false)
         , with_idx_buffer(false)
         , with_alpha     (false)
         , texture_type   (TT_none)
         , drawing_style  (DS_point)
      {}

      bool   is_wireframe
           , with_idx_buffer
           , with_alpha;

      texture_type_t texture_type ;
      render_style_t drawing_style;
   };

   /* render_t
    * this class contains render settings and methods, that use in object rendering.
    */

   class render_t
   {
   public:
      render_t();
      ~render_t();

      static effect_t create_effect( size_t id, material_ptr_t const & material, texture_ptr_t const & texture = texture_ptr_t() );

      render_param_t render_param_;

   private:
      effects_t effects_;
   };
}