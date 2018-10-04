#pragma once

#include <memory>
#include <frame/scene/camera/camera.hpp>

namespace LE {
   namespace Utils {
      class user_camera_t {
      public:
         user_camera_t(camera_ptr_t const & camera);

         void process_mouse_press(int x_pos, int y_pos, bool is_left_button, bool is_pressed);
         virtual void process_cursore_movement(int x_pos, int y_pos) = 0;
         virtual void process_keyboard(bool const (&key_buffer)[256]) = 0;
         virtual void process_wheel(int scrolls_count) = 0;

         void set_camera_cursour_sensetivity(float sensetivity);
         void set_camera_keyboard_sensetivity(float sensetivity);
         void set_camera_wheel_sensetivity(float sensetivity);

      protected:
         float camera_cursour_sensetivity_, camera_keyboard_sensetivity_, camera_wheel_sensetivity_;
         camera_ptr_t camera_;
         int old_x_, old_y_;
         bool is_mouse_left_button_pressed_, is_mouse_right_button_pressed_;
      };

      typedef std::unique_ptr<user_camera_t> user_camera_ptr_t;

      class user_wasd_camera_t : public user_camera_t {
      public:
         user_wasd_camera_t(camera_ptr_t const & camera);

         void process_cursore_movement(int x_pos, int y_pos);
         void process_keyboard(bool const (&key_buffer)[256]);
         void process_wheel(int scrolls_count) {}

      };

      class user_mouse_camera_t : public user_camera_t {
      public:
         user_mouse_camera_t(camera_ptr_t const & camera);

         void process_cursore_movement(int x_pos, int y_pos);
         void process_keyboard(bool const (&key_buffer)[256]) {};
         void process_wheel(int scrolls_count);
      };
   }
}