#pragma once

#include "../../base.h"

namespace obj_manager
{
   using namespace object;

   class obj_manager_t
   {
   public:
      static obj_ptr_t create_triangle( effect_t const & effect, buffer_manager_ptr_t const & vbo_manager
                                       ,vec4 color = vec4(1.0f), vec3 pos1 = vec3()
                                       ,vec3 pos2 = vec3(1.0f, 0.0f, 0.0f), vec3 pos3 = vec3(0.0f, 1.0f, 0.0f)
                                       ,vec3 pos = vec3() );

      static obj_ptr_t create_quad( effect_t const & effect
                                   ,buffer_manager_ptr_t const & vbo_manager
                                   ,render_param_t & render_param, vec4 color = vec4(1.0f)
                                   ,vec3 pos1 = vec3(), vec3 pos2 = vec3(0.0f, 1.0f, 0.0f)
                                   ,vec3 pos3 = vec3(1.0f, 0.0f, 0.0f), vec3 pos4 = vec3(1.0f, 1.0f, 0.0f)
                                   ,vec3 pos = vec3(), float size = 1.0f );

      static obj_ptr_t create_plane( effect_t const & effect
                                    ,buffer_manager_ptr_t const & vbo_manager
                                    ,float size_x, float size_z, int frequency, vec4 color = vec4(1.0f) );

      static obj_ptr_t create_sky_box( effect_t const & effect
                                      ,buffer_manager_ptr_t const & buffer_manager, float size = 1.0f );

      static obj_ptr_t create_line( effect_t const & effect
                                   ,buffer_manager_ptr_t const & vbo_manager
                                   ,vec4 color, vec3 from, vec3 to );

      static obj_ptr_t create_sphere( effect_t const & effect
                                     ,buffer_manager_ptr_t const & buffer_manager
                                     ,vec4 color = vec4(1.0f), size_t detail_level = 1, vec3 pos = vec3()
                                     ,float size = 1.0f, texture_type_t texture_type = TT_none );

      static obj_ptr_t create_frustrum( effect_t const & effect
                                       ,buffer_manager_ptr_t const & buffer_manager
                                       ,env_ptr_t const & env, vec4 color = vec4(1.0f), float size = 1.0f );

      static obj_ptr_t create_box( effect_t const & effect
                                  ,buffer_manager_ptr_t const & buffer_manager
                                  ,vec4 color = vec4(1.0f), vec3 pos = vec3(), float size = 1.0f
                                  ,texture_type_t texture_type = TT_none );

      static obj_ptr_t create_point( effect_t const & effect
                                    ,buffer_manager_ptr_t const & buffer_manager
                                    ,render_param_t const & render_param, vec4 color = vec4(1.0f)
                                    ,vec3 pos = vec3(), float size = 1.0f );

      static obj_ptr_t load_model( effect_t const & effect
                                  ,buffer_manager_ptr_t const & buffer_manager
                                  ,render_param_t const & render_param, const string & file_name, vec4 color
                                  ,vec3 pos = vec3(), float size = 1.0f );

   private:
   };

}