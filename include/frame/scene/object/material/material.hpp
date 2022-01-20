#pragma once

#include <vector>
#include <functional>

#include <frame/scene/object/material/texture/texture.hpp>
#include <frame/scene/object/material/shader/shader.hpp>

namespace LE {
   class material_t {
   public:
      material_t();

      void add_texture(texture_ptr_t const& texture);
      void set_shader_prog(shader_prog_ptr_t const& shader_prog);

      texture_ptr_t texture(size_t i) const;
      std::vector<texture_ptr_t> texture() const;
      shader_prog_ptr_t shader_prog() const;

      void bind();
      void unbind();

      typedef std::function<void(shader_prog_ptr_t const&)> set_uniforms_callback_t;
      void set_uniforms_callback(set_uniforms_callback_t const& callback);
      set_uniforms_callback_t set_uniforms_callback();

   private:
      std::vector<texture_ptr_t> textures_;
      shader_prog_ptr_t shader_prog_;
      set_uniforms_callback_t set_uniforms_callback_;
   };

   typedef std::shared_ptr<material_t> material_ptr_t;
}
