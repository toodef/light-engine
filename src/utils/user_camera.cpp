#include <utils/user_camera.hpp>

using namespace LE;
using namespace LE::Utils;

user_camera_t::user_camera_t(camera_ptr_t const & camera): 
   camera_(camera),
   camera_cursour_sensetivity_(0.001f), camera_keyboard_sensetivity_(0.0001f), camera_wheel_sensetivity_(0.01f),
   is_mouse_left_button_pressed_(false), is_mouse_right_button_pressed_(false)
{}

void user_camera_t::process_mouse_press(int x_pos, int y_pos, bool is_left_button, bool is_pressed) {
   if (is_left_button)
      is_mouse_left_button_pressed_ = is_pressed;
   else
      is_mouse_right_button_pressed_ = is_pressed;

   old_x_ = x_pos;
   old_y_ = y_pos;
}

void user_camera_t::set_camera_cursour_sensetivity(float sensetivity) { camera_cursour_sensetivity_ = sensetivity; }
void user_camera_t::set_camera_keyboard_sensetivity(float sensetivity) { camera_keyboard_sensetivity_ = sensetivity; }
void user_camera_t::set_camera_wheel_sensetivity(float sensetivity) { camera_wheel_sensetivity_ = sensetivity; }

user_wasd_camera_t::user_wasd_camera_t(camera_ptr_t const & camera) : user_camera_t(camera) {}

void user_wasd_camera_t::process_cursore_movement(int x_pos, int y_pos) {
   if (is_mouse_left_button_pressed_) {
      camera_->rotate(float(x_pos - old_x_) * camera_cursour_sensetivity_, camera_->up());
      camera_->rotate(float(y_pos - old_y_) * camera_cursour_sensetivity_, camera_->right());
      old_x_ = x_pos;
      old_y_ = y_pos;
   }
}

void user_wasd_camera_t::process_keyboard(bool const (&key_buffer)[256]) {
   if (key_buffer['w']) camera_->move(camera_->dir() * camera_keyboard_sensetivity_);
   if (key_buffer['a']) camera_->move(-1.f * camera_->right() * camera_keyboard_sensetivity_);
   if (key_buffer['s']) camera_->move(-1.f * camera_->dir() * camera_keyboard_sensetivity_);
   if (key_buffer['d']) camera_->move(camera_->right() * camera_keyboard_sensetivity_);
   if (key_buffer['q']) camera_->move(camera_->up() * camera_keyboard_sensetivity_);
   if (key_buffer['e']) camera_->move(-1.f * camera_->up() * camera_keyboard_sensetivity_);
}

user_mouse_camera_t::user_mouse_camera_t(camera_ptr_t const & camera) : user_camera_t(camera) { }

void user_mouse_camera_t::process_cursore_movement(int x_pos, int y_pos) {
   if (is_mouse_left_button_pressed_) {
      glm::vec3 old_look_at = camera_->look_at();

      camera_->rotate(float(x_pos - old_x_) * camera_cursour_sensetivity_, camera_->up());
      camera_->rotate(float(y_pos - old_y_) * camera_cursour_sensetivity_, camera_->right());

      camera_->move(old_look_at - camera_->look_at());
   }

   if (is_mouse_right_button_pressed_) {
      camera_->move(glm::normalize(camera_->up() * float(y_pos - old_y_) + camera_->right() * float(x_pos - old_x_)) * camera_cursour_sensetivity_);
   }

   if (is_mouse_left_button_pressed_ || is_mouse_right_button_pressed_) {
      old_x_ = x_pos;
      old_y_ = y_pos;
   }
}

void user_mouse_camera_t::process_wheel(int scrolls_count) {
   glm::vec3 old_look_at = camera_->look_at();
   camera_->move(camera_->dir() * float(scrolls_count) * camera_wheel_sensetivity_);
   camera_->look_at(old_look_at);
}
