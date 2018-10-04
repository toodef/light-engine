#pragma once

#include <vector>
#include <memory>

#include <glm/vec3.hpp>

#include "scene/scene.hpp"

namespace LE {
   class frame_t {
   public:
      frame_t();

      void set_background_color(glm::vec3 const & color);
      void draw();
      void resize(unsigned int width, unsigned int height);
      void enable_depth_test(bool is_enable);

      void add_scene(scene_ptr_t const & scene);

   private:
      std::vector<scene_ptr_t> scenes_;
      bool depth_test_enabled_;
   };

   typedef std::shared_ptr<frame_t> frame_ptr_t;
}