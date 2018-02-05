#pragma once

#include <chrono>
#include <optional>

#include "../base.h"

namespace environment
{
   using namespace object;

   class timer_t
   {
   public:

      timer_t()
      {
         restart();
      }

      void restart()
      {
         start_time_ = std::chrono::system_clock::now();
      }

      template<typename T> T time()
      {
         std::chrono::duration<T> res = chrono::system_clock::now() - start_time_;
         return res.count();
      }

      template<typename T> T time_msec()
      {
         std::chrono::duration<T> res = chrono::system_clock::now() - start_time_;

         return res.count() * 1000.0f;
      }

   private:
      std::chrono::system_clock::time_point start_time_;
   };

   class camera_t
   {
   public:
      camera_t( vec3 const & pos = vec3(), vec3 const & look_at = vec3(0, 0, 1)
               ,quat const & q = quat(1, 0, 0, 0), vec3 const & dir = vec3(0, 0, -1)
               ,vec3 const & up = vec3(0, 1, 0), vec3 const & right = vec3(1, 0, 0));

      vec3 const & pos        () const;
      vec3 const & look_at    () const;
      quat const & orientation() const;

      virtual vec3 const & dir  () const;
      virtual vec3 const & up   () const;
      virtual vec3 const & right() const;

      virtual void set_dir  ( vec3 const & dir );
      virtual void set_up   ( vec3 const & up );
      virtual void set_right( vec3 const & right );

      mat4 world_view() const;

      virtual void set_pos        ( vec3 const & pos );
      virtual void set_look_at    ( vec3 const & look_at, bool dir );
      virtual void set_orientation( quat const & q );

   protected:
      struct dep_data_t
      {
         vec3 dir
             ,up
             ,right;
      };

      std::optional<dep_data_t> dep_data_;

      vec3 pos_
          ,look_at_;

      quat q_;
   };

   class fake_camera_t : public camera_t
   {
   public:
      fake_camera_t( const camera_t & cam, float view_angle, float asp, float z_near, float z_far );

      void update_frustum( float view_angle, float asp, float near, float far_ );

      /* get frustum vertex by number
       * from near to far; go-round from left bottom vertex counterclockwise
       * when viewed from the direction of the vector 'dir'
       */
      vec3 frustum_vertex( unsigned int id );

      obj_ptr_t frustum() const;

      void set_frustum( obj_ptr_t const & frustum );

      // frustum parameters
      float view_angle() const;
      float near_plane() const;
      float far_plane () const;
      float asp       () const;

      // frustum plane normals
      vec3 n_near  () const;
      vec3 n_far   () const;
      vec3 n_left  () const;
      vec3 n_right () const;
      vec3 n_top   () const;
      vec3 n_bottom() const;

      // frustum plane distance
      float d_near  () const;
      float d_far   () const;
      float d_left  () const;
      float d_right () const;
      float d_top   () const;
      float d_bottom() const;

      void set_view_angle( float new_va );

   private:
      // frustum plane normals
      vec3  n_near_
           ,n_far_
           ,n_left_
           ,n_right_
           ,n_top_
           ,n_bottom_;

      // frustum plane distance
      float  d_near_
            ,d_far_
            ,d_left_
            ,d_right_
            ,d_top_
            ,d_bottom_;

      vector<vec3> frustum_coord_;

      float  view_angle_
            ,near_
            ,far_
            ,asp_; // width / height

      bool enabled_;

      // frustum drawing object
      obj_ptr_t frustum_;
   };

   struct env_t
   {
      env_t( string const & window_handle, material_manager_ptr_t material_manager, texture_manager_ptr_t texture_manager, camera_t cam, int width, int height, float view_angle, float z_near, float z_far );

      camera_t      cam_;
      fake_camera_t fake_cam_;

      // screen size
      int  width_
          ,height_;

      effect_t default_effect_;

      // perspective transform matrices
      mat4 proj_;
      mat4 view_;
      mat4 mvp_ ;

      material_manager_ptr_t material_manager_;
      texture_manager_ptr_t  texture_manager_ ;
      buffer_manager_ptr_t   buffer_manager_  ;
   };
}