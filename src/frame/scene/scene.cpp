#include "../../../include/frame/scene/scene.hpp"

using namespace LE;

scene_t::scene_t() : camera_(std::make_shared<camera_t>()) {
   // TOOD: optimize, recalc mvp for every object
   set_uniform_variables_callback_ = [this](shader_prog_ptr_t const & shader_prog) {
      shader_prog->uniform_variable("mvp")->set(camera_->model_view_projection_matrix());
   };
}

void scene_t::add_object(object_ptr_t const & object) {
   object->set_uniforms_callback(set_uniform_variables_callback_);
   objects_.push_back(object);
}

void scene_t::draw() const {
   for (object_ptr_t const & object : objects_)
      object->draw();
}

void scene_t::set_camera(camera_ptr_t const & camera) { camera_ = camera; }
camera_ptr_t scene_t::get_camera() const { return camera_;  }
