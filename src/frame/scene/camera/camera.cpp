#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

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
   static const glm::quat q = glm::quat(1, 0, 0, 0);
   glm::quat rotate = glm::rotate(q, angle, axis);
   set_orientation(rotate);
}

void camera_t::move(glm::vec3 const & offset) {
   pos_ += offset;
   look_at_ += offset;
   model_view_matrix_is_up_to_date_ = false;
   mvp_mat_is_up_to_date_ = false;
}

glm::vec3 camera_t::dir() const { return dir_; }
glm::vec3 camera_t::up() const { return up_; }
glm::vec3 camera_t::right() const { return right_; }
glm::vec3 camera_t::look_at() const { return look_at_; }
glm::vec3 camera_t::pos() const { return pos_; }
void camera_t::dir(glm::vec3 const & dir) { dir_ = dir; }
void camera_t::up(glm::vec3 const & up) { up_ = up; model_view_matrix_is_up_to_date_ = false; mvp_mat_is_up_to_date_ = false; }
void camera_t::right(glm::vec3 const & right) { right_ = right; }
void camera_t::look_at(glm::vec3 const & look_at) { look_at_ = look_at; model_view_matrix_is_up_to_date_ = false; mvp_mat_is_up_to_date_ = false; }
void camera_t::pos(glm::vec3 const & pos) { pos_ = pos; model_view_matrix_is_up_to_date_ = false; mvp_mat_is_up_to_date_ = false; }

unsigned int camera_t::height() const { return height_; }
unsigned int camera_t::width() const { return width_; }
void camera_t::height(unsigned int height) { height_ = height; projection_matrix_is_up_to_date_ = false; mvp_mat_is_up_to_date_ = false; }
void camera_t::width(unsigned int width) { width_ = width; projection_matrix_is_up_to_date_ = false; mvp_mat_is_up_to_date_ = false; }

glm::mat4 camera_t::projection_matrix() {
   if (!projection_matrix_is_up_to_date_) {
      prj_mat_ = glm::perspective(view_angle_, (float)width_ / height_, z_near_, z_far_);
      projection_matrix_is_up_to_date_ = true;
      mvp_mat_is_up_to_date_ = false;
   }
   return prj_mat_;
}

glm::mat4 camera_t::model_view_matrix() {
   if (!model_view_matrix_is_up_to_date_) {
      model_view_mat_ = glm::lookAt(pos_, look_at_, up_);
      model_view_matrix_is_up_to_date_ = true;
      mvp_mat_is_up_to_date_ = false;
      normal_mat_is_up_to_date_ = false;
   }
   return model_view_mat_;
}

glm::mat4 camera_t::model_view_projection_matrix() {
   if (!mvp_mat_is_up_to_date_) {
      mvp_mat_ = projection_matrix() * model_view_matrix();
      mvp_mat_is_up_to_date_ = true;
   }
   return mvp_mat_;
}

glm::mat4 camera_t::normal_matrix() {
   if (!normal_mat_is_up_to_date_)
      normal_mat_ = glm::transpose(glm::inverse(model_view_matrix()));
   return normal_mat_;
}

glm::mat4 camera_t::billboard_matrix(glm::vec3 const & camera_pos) {
   glm::mat4 model_view = model_view_matrix();
   //model_view = {
   //  {1, 0, 0, pos_.x}, // First column
   //  {0, 1, 0, pos_.y}, // Second column
   //  {0, 0, 1, pos_.z}, // Third column
   //  {0, 0, 0, 1}  // Forth column
   //};
   //model_view = glm::lookAt(pos_, glm::vec3(0, 0, 1), glm::vec3(0, -1, 0));

   model_view[0][0] = 1.f;
   model_view[0][1] = 0.f;
   model_view[0][2] = 0.f;
   model_view[1][0] = 0.f;
   model_view[1][1] = 1.f;
   model_view[1][2] = 0.f;

   //have no idea why
   //model_view[2][0] = 0.f;
   //model_view[2][1] = 0.f;
   //model_view[2][2] = 1.f;

   return projection_matrix() * model_view;
}

void camera_t::set_orientation(glm::quat const & q)
{
   dir(q * dir_);
   look_at(q * (look_at_ - pos_) + pos_);
   right(q * right_);
   up(q * up_);
   q_ = q;
   model_view_matrix_is_up_to_date_ = false;
   mvp_mat_is_up_to_date_ = false;
}
