#include <frame/scene/object/builtin_objects.hpp>

using namespace LE;

triangle_t::triangle_t(std::array<glm::vec3, 3> const& vertices): vertices_(vertices) {}

void triangle_t::set_shader_prog(shader_prog_ptr_t const& prog) { shader_prog_ = prog; }
void triangle_t::set_texture(texture_ptr_t const& texture) { texture_ = texture; }
void triangle_t::set_color(glm::vec3 const& color) { color_ = color; color_was_set_ = true; }

object_ptr_t triangle_t::compile() const {
   buffer_ptr_t buffer;
   
   if (color_was_set_)
      buffer = std::make_shared<buffer_t>(std::vector<glm::vec3>{ vertices_[0], vertices_[1], vertices_[2] }, color_);
   else
      buffer = std::make_shared<buffer_t>(std::vector<glm::vec3>{ vertices_[0], vertices_[1], vertices_[2] });
   auto object = std::make_shared<object_t>(buffer, shader_prog_, std::vector<texture_ptr_t>{ texture_ });
   object->set_drawing_style(object_t::DS_triangles);
   object->enable_face_culling(false, false);
   return object;
}
