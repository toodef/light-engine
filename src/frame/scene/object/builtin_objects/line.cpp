#include <frame/scene/object/builtin_objects.hpp>

using namespace LE;

line_t::line_t(glm::vec3 const& v0, glm::vec3 const& v1) : v0_(v0), v1_(v1) {}

void line_t::set_shader_prog(shader_prog_ptr_t const& prog) { shader_prog_ = prog; }
void line_t::set_texture(texture_ptr_t const& texture) { texture_ = texture; }
void line_t::set_color(glm::vec3 const& color) { color_ = color; color_was_set_ = true; }

object_ptr_t line_t::compile() const {
   const std::vector<glm::vec3> vertices = { v0_, v1_ };
   
   buffer_ptr_t buffer;
   if (color_was_set_)
      buffer = std::make_shared<buffer_t>(vertices, color_);
   else
      buffer = std::make_shared<buffer_t>(vertices);

   auto object = std::make_shared<object_t>(buffer, shader_prog_, std::vector<texture_ptr_t>{ texture_ });
   object->set_drawing_style(object_t::DS_lines);
   object->enable_face_culling(false, false);
   return object;
}
