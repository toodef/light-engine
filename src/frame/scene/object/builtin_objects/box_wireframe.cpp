#include <frame/scene/object/builtin_objects.hpp>

using namespace LE;

box_wireframe_t::box_wireframe_t(glm::vec3 const& center, glm::vec3 const& up, glm::vec3 const& right, glm::vec3 const& forward) :
   box_t(center, up, right, forward)
{}

object_ptr_t box_wireframe_t::compile() const {
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
      0, 1, 1, 2, 2, 3, 3, 0,
      4, 5, 5, 6, 6, 7, 7, 4,
      0, 4, 1, 5, 2, 6, 3, 7
   };

   buffer_ptr_t buffer;
   if (color_was_set_)
      buffer = std::make_shared<buffer_t>(vertices, color_);
   else
      buffer = std::make_shared<buffer_t>(vertices);

   buffer->add_index_buffer(indices);
   auto object = std::make_shared<object_t>(buffer, shader_prog_);
   object->set_drawing_style(object_t::DS_lines);
   return object;
}
