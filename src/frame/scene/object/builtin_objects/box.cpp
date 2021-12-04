#include <frame/scene/object/builtin_objects.hpp>

using namespace LE;

box_t::box_t(glm::vec3 const& center, glm::vec3 const& up, glm::vec3 const& right, glm::vec3 const& forward) :
   center_(center), up_(up), right_(right), forward_(forward) {}

void box_t::set_shader_prog(shader_prog_ptr_t const& prog) { shader_prog_ = prog; }
void box_t::set_texture(texture_ptr_t const& texture) { texture_ = texture; }
void box_t::set_color(glm::vec3 const& color) { color_ = color; color_was_set_ = true; }

object_ptr_t box_t::compile() const {
   const std::vector<glm::vec3> vertices = {
      center_ - up_ - right_ - forward_,
      center_ + up_ - right_ - forward_,
      center_ + up_ + right_ - forward_,
      center_ - up_ + right_ - forward_,
      center_ - up_ - right_ + forward_,
      center_ + up_ - right_ + forward_,
      center_ + up_ + right_ + forward_,
      center_ - up_ + right_ + forward_
   };

   static const std::vector<unsigned int> indices{
      0, 1, 2, 0, 2, 3,
      0, 1, 5, 0, 5, 4,
      4, 5, 6, 4, 6, 7,
      3, 2, 6, 3, 6, 7,
      1, 5, 6, 1, 6, 2,
      4, 0, 3, 4, 3, 7
   };

   buffer_ptr_t buffer;
   if (color_was_set_)
      buffer = std::make_shared<buffer_t>(vertices, color_);
   else
      buffer = std::make_shared<buffer_t>(vertices);

   buffer->add_index_buffer(indices);
   auto object = std::make_shared<object_t>(buffer, shader_prog_, texture_);
   object->set_drawing_style(object_t::DS_triangles);
   return object;
}
