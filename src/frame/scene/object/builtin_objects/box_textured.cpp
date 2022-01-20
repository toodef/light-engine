#include <frame/scene/object/builtin_objects.hpp>

using namespace LE;

box_textured_t::box_textured_t(glm::vec3 const& center, glm::vec3 const& up, glm::vec3 const& right, glm::vec3 const& forward) :
   box_t(center, up, right, forward)
{}

void box_textured_t::set_texture(texture_ptr_t const & texture) { texture_ = texture; }

object_ptr_t box_textured_t::compile() const {
   const std::vector<glm::vec3> vertices = {
      center_ - up_ - right_ - forward_, // 0
      center_ + up_ - right_ - forward_, // 1
      center_ + up_ + right_ - forward_, // 2
      center_ - up_ + right_ - forward_, // 3
      center_ - up_ - right_ + forward_, // 4
      center_ + up_ - right_ + forward_, // 5
      center_ + up_ + right_ + forward_, // 6
      center_ - up_ + right_ + forward_, // 7
      center_ - up_ - right_ + forward_, // 8 (4)
      center_ - up_ - right_ + forward_, // 9 (4)
      center_ + up_ - right_ + forward_, // 10 (5)
      center_ + up_ - right_ + forward_, // 11 (5)
      center_ + up_ + right_ + forward_, // 12 (6)
      center_ - up_ + right_ + forward_  // 13 (7)
   };

   static const std::vector<unsigned int> indices {
      0, 1, 2, 0, 2, 3,
      0, 5, 1, 0, 4, 5,
      8, 6, 10, 8, 7, 6,
      3, 2, 6, 3, 6, 7,
      1, 11, 12, 1, 12, 2,
      9, 0, 3, 9, 3, 13
   };

   static const std::vector<glm::vec2> tex_coords = {
      {0.25, 2.f / 3.f}, {0.25, 1.f / 3.f},
      {0.5, 1.f / 3.f}, {0.5, 2.f / 3.f},
      {0, 2.f / 3.f}, {0, 1.f / 3.f},
      {0.75, 1.f / 3.f}, {0.75, 2.f / 3.f},
      {1, 2.f / 3.f}, {0.25, 1},
      {1, 1.f / 3.f}, {0.25, 0},
      {0.5, 0}, {0.5, 1}
   };

   buffer_ptr_t buffer;
   if (color_was_set_)
      buffer = std::make_shared<buffer_t>(vertices, color_, tex_coords);
   else
      buffer = std::make_shared<buffer_t>(vertices, tex_coords);

   buffer->add_index_buffer(indices);
   auto object = std::make_shared<object_t>(buffer, shader_prog_, std::vector<texture_ptr_t>{ texture_ });
   object->set_drawing_style(object_t::DS_triangles);
   return object;
}
