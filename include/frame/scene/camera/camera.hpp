#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include <memory>

namespace LE {
   class camera_t {
   public:
      camera_t(glm::vec3 const & pos = glm::vec3(0, 0, -1), glm::vec3 const & dir = glm::vec3(0, 0, 1), glm::vec3 const & up = glm::vec3(0, 1, 0));

      void rotate(float angle, glm::vec3 const & axis);
      void move(glm::vec3 const & offset);

      glm::vec3 dir() const;
      glm::vec3 up() const;
      glm::vec3 right() const;
      glm::vec3 look_at() const;
      glm::vec3 pos() const;
      void dir(glm::vec3 const & dir);
      void up(glm::vec3 const & up);
      void right(glm::vec3 const & right);
      void look_at(glm::vec3 const & look_at);
      void pos(glm::vec3 const & pos);

      unsigned int height() const;
      unsigned int width() const;
      void height(unsigned int height);
      void width(unsigned int width);

      float z_far() const;
      void z_far(float z_far);
      float z_near() const;
      void z_near(float z_near);

      glm::mat4 projection_matrix();
      glm::mat4 model_view_matrix();
      glm::mat4 model_view_projection_matrix();
      glm::mat4 normal_matrix();
      glm::mat4 billboard_matrix(glm::vec3 const& camera_pos);

   private:
      void set_orientation(glm::quat const & q);

   private:
      glm::vec3 dir_, up_, right_, look_at_, pos_;
      glm::quat q_;
      glm::mat4 prj_mat_, model_view_mat_, mvp_mat_, normal_mat_;

      unsigned int width_, height_;
      float view_angle_, z_near_, z_far_;

      bool look_at_is_locked_,
           projection_matrix_is_up_to_date_, model_view_matrix_is_up_to_date_, mvp_mat_is_up_to_date_, normal_mat_is_up_to_date_;
   };

   typedef std::shared_ptr<camera_t> camera_ptr_t;
}
