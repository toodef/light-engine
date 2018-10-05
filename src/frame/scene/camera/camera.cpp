#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../../../../include/frame/scene/camera/camera.hpp"

using namespace LE;

camera_t::camera_t(glm::vec3 const & pos, glm::vec3 const & dir, glm::vec3 const & up):
   pos_(pos), dir_(dir), up_(up),
   z_near_(0.1f), z_far_(100.f), view_angle_(20.f), width_(1), height_(1),
   projection_matrix_is_up_to_date_(false), model_view_matrix_is_up_to_date_(false), mvp_mat_is_up_to_date_(false)
{
   right_ = glm::cross(dir_, up_);
   look_at_ = dir;
}

void camera_t::rotate(float angle, glm::vec3 const & axis) {
   glm::quat rotate = glm::rotate(glm::quat(), angle, axis);
   set_orientation(rotate);
}

void camera_t::move(glm::vec3 const & offset) {
   pos_ += offset;
   look_at_ += offset;
   model_view_matrix_is_up_to_date_ = false;
}

glm::vec3 camera_t::dir() const { return dir_; }
glm::vec3 camera_t::up() const { return up_; }
glm::vec3 camera_t::right() const { return right_; }
glm::vec3 camera_t::look_at() const { return look_at_; }
glm::vec3 camera_t::pos() const { return pos_; }
void camera_t::dir(glm::vec3 const & dir) { dir_ = dir; }
void camera_t::up(glm::vec3 const & up) { up_ = up; model_view_matrix_is_up_to_date_ = false; }
void camera_t::right(glm::vec3 const & right) { right_ = right; }
void camera_t::look_at(glm::vec3 const & look_at) { look_at_ = look_at; model_view_matrix_is_up_to_date_ = false; }
void camera_t::pos(glm::vec3 const & pos) { pos_ = pos; model_view_matrix_is_up_to_date_ = false; }

unsigned int camera_t::height() const { return height_; }
unsigned int camera_t::width() const { return width_; }
void camera_t::height(unsigned int height) { height_ = height; projection_matrix_is_up_to_date_ = false; }
void camera_t::width(unsigned int width) { width_ = width; projection_matrix_is_up_to_date_ = false; }

glm::mat4 camera_t::projection_matrix() {
   if (!projection_matrix_is_up_to_date_) {
      prj_mat_ = glm::perspective(view_angle_, (float)width_ / height_, z_near_, z_far_);
      projection_matrix_is_up_to_date_ = true;
      mvp_mat_is_up_to_date_ = false;
   }
   return prj_mat_;
}

glm::mat4 camera_t::world_view_matrix() {
   if (!model_view_matrix_is_up_to_date_) {
      model_view_mat_ = glm::lookAt(pos_, look_at_, up_);
      model_view_matrix_is_up_to_date_ = true;
      mvp_mat_is_up_to_date_ = false;
   }
   return model_view_mat_;
}

glm::mat4 camera_t::model_view_projection_matrix() {
   if (!mvp_mat_is_up_to_date_)
      mvp_mat_ = projection_matrix() * world_view_matrix();
   return mvp_mat_;
}

void camera_t::set_orientation(glm::quat const & q)
{
   dir(q * dir_);
   look_at(q * (look_at_ - pos_) + pos_);
   right(q * right_);
   up(q * up_);
   q_ = q;
   model_view_matrix_is_up_to_date_ = false;
}
