#include <frame/scene/object/material/material.hpp>

using namespace LE;

material_t::material_t() : shader_prog_(nullptr) {}

void material_t::add_texture(texture_ptr_t const& texture) { textures_.push_back(texture); }
void material_t::set_shader_prog(shader_prog_ptr_t const& shader_prog) { shader_prog_ = shader_prog; }

texture_ptr_t material_t::texture(size_t i) const { return textures_[i]; }
std::vector<texture_ptr_t> material_t::texture() const { return textures_; };
shader_prog_ptr_t material_t::shader_prog() const { return shader_prog_; }

void material_t::bind() {
   shader_prog_->bind();
   if (texture_)
      texture_->bind();

   if (set_uniforms_callback_)
      set_uniforms_callback_(shader_prog_);
}

void material_t::unbind() {
   if (texture_)
      texture_->unbind();
   shader_prog_->unbind();
}

void material_t::set_uniforms_callback(set_uniforms_callback_t const& callback) { set_uniforms_callback_ = callback; }
set_uniforms_callback_t material_t::set_uniforms_callback() { return set_uniforms_callback_; }
