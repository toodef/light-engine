#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <vector>
#include <memory>

#include <frame/scene/object/object.hpp>
#include <frame/scene/camera/camera.hpp>
#include <frame/scene/object/material/material.hpp>

namespace LE {
   class scene_t {
   public:
      scene_t();

      void add_object(object_ptr_t const & object);
      void draw() const;

      void set_camera(camera_ptr_t const & camera);
      camera_ptr_t get_camera() const;

      material_t::set_uniforms_callback_t get_set_uniform_callback();

   private:
      std::vector<object_ptr_t> objects_;
      camera_ptr_t camera_;
      material_t::set_uniforms_callback_t set_uniform_variables_callback_;
   };

   typedef std::shared_ptr<scene_t> scene_ptr_t;
}
