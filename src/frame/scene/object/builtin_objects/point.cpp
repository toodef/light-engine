#include <frame/scene/object/builtin_objects.hpp>

using namespace LE;

point_t::point_t(glm::vec3 const& pos): pos_(pos) {}

void point_t::set_shader_prog(shader_prog_ptr_t const& prog) { shader_prog_ = prog; }
void point_t::set_texture(texture_ptr_t const& texture) { texture_ = texture; }
void point_t::set_color(glm::vec3 const& color) { color_ = color; color_was_set_ = true; }

object_ptr_t point_t::compile() const {
   buffer_ptr_t buffer = std::make_shared<buffer_t>(std::vector<glm::vec3>{pos_}, color_);
   return std::make_shared<object_t>(buffer, shader_prog_, std::vector<texture_ptr_t>{ texture_ });
}
