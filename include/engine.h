#pragma once

#include "../src/base.h"

namespace SE
{
   using namespace scene;
   using namespace obj_manager;
   using namespace environment;

   class engine_t;

   typedef std::shared_ptr<engine_t> render_ptr_t;

   typedef vector<material_ptr_t> materials_t;
   typedef vector<texture_ptr_t>  textures_t;
   typedef vector<buffer_ptr_t>   vertex_buffers_t;

   typedef enum
   {
        CW_front = 1
      , CW_back
      , CW_right
      , CW_left
      , CW_up
      , CW_down
   } camera_way_t;

   class engine_t
   {
   public:
       static engine_t & instance();

       void init( int argc, char ** argv, string const & window_name );

      /*
       * perspective transform functions
       */

      // update perspective transform matrices
      void update_proj();
      void update_mvp ();

      // set screen parameters
      void set_width ( int width  );
      void set_height( int height );

      // get screen parameters
      int  width () const;
      int  height() const;

      // frustum manage functions
      void update_frustum();
      void draw_frustum  ( bool is_need_draw_frustum );
      bool frustum_state () const;

      /*
       * camera functions
       */

      void rotate_camera( float heading, float pitch );

      void move_camera( const vec3 & way );
      void move_camera( float scale, camera_way_t cam_way );

      void set_cam_pos( vec3 const & pos     );
      void set_look_at( vec3 const & look_at );

      // get camera
      camera_t cam() const;

      /*
       * fake camera functions
       */

      void rotate_fake_cam( float heading, float pitch );

      void move_fake_camera( const vec3 & way );
      void move_fake_camera( float scale, camera_way_t cam_way );

      void enable_fake_cam ( bool need_fake_cam );

      // enabled/disabled
      bool fake_cam_state() const;

      // set view angle for fake cam
      void set_fake_cam_va( float new_va );

      // get view angle for fake cam
      float view_angle() const;

      // update fake cam
      void update_fake_cam() const;

      /*
       * scene functions
       */

      scene_t & scene       ();
      scene_t & static_scene();
      scene_t & double_scene();

      /*
       * effect functions
       */

      // attach effect to object
      void set_effect( effect_t const & effect, unsigned int object_id );

      /*
       * operation with material function
       */

      material_ptr_t create_material( const string & vertex_shader, const string & fragment_shader
                                     ,const string & geomerty_shader = "" );

      /*
       * operation with texture functions
       */

      texture_ptr_t create_texture( tex_params_t const & params );
      texture_ptr_t create_texture( GLenum type );
      texture_ptr_t create_texture();

      /*
       * operation with buffer function
       */

      buffer_ptr_t create_buffer( vertices_t data, size_t size, GLenum usage_type = GL_STATIC_DRAW );

      /*
       * effect components managers functions
       */

      material_manager_ptr_t get_material_manager() const;
      texture_manager_ptr_t  get_texture_manager () const;
      buffer_manager_t  * get_buffer_manager  () const;

      /*
       *timer function
       */

      float fps();

       /*
        * GLUT events functions
        */

       typedef void (* on_display_fnc_t)();
       void set_on_display(on_display_fnc_t on_display_fnc);
       void on_display();

      /*
       * other functions
       */

      void draw_frame ();

      effect_t default_effect() const;

   private:
       engine_t();
       ~engine_t();

       engine_t( engine_t const & );
       engine_t & operator= ( engine_t const & );

   private:
       on_display_fnc_t on_display_fnc_;

      material_manager_ptr_t material_manager_;
      texture_manager_ptr_t  texture_manager_;
      buffer_manager_ptr_t   buffer_manager_;

      env_ptr_t env_;

      float   view_angle_
            , near_
            , far_;

      scene::scene_t   scene_
                     , static_scene_
                     , double_scene_;

      bool   draw_frustum_
           , enable_fake_cam_;

      buffer_ptr_t   bounding_box_buffer_
                   , tf_buffer_;

      int * visible_boxes_indexes_;

      vertex_t * bound_boxes_;

      environment::timer_t timer_;

      float fps_, delta_time_;

      size_t accum_frames_count_;
   };

    static engine_t & engine = engine_t::instance();
}
