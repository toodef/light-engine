#include <frame/scene/object/builtin_objects.hpp>

using namespace LE;

quad_t::quad_t(std::array<glm::vec3, 4> const& vertices) : vertices_(vertices) {}

void quad_t::set_shader_prog(shader_prog_ptr_t const& prog) { shader_prog_ = prog; }
void quad_t::set_texture(texture_ptr_t const& texture) { texture_ = texture; }
void quad_t::set_color(glm::vec3 const& color) { color_ = color; color_was_set_ = true; }

object_ptr_t quad_t::compile() const {
   static const std::vector<unsigned int> indices {
      0 , 1 , 2 ,
      0 , 2 , 3 };

   std::vector<glm::vec2> tex_coords = { glm::vec2(1.f, 1.f), glm::vec2(1.f, 0.f), glm::vec2(0.f, 0.f), glm::vec2(0.f, 1.f) };

   buffer_ptr_t buffer;
   if (color_was_set_)
      buffer = std::make_shared<buffer_t>(std::vector<glm::vec3>{ vertices_[0], vertices_[1], vertices_[2], vertices_[3] }, color_, tex_coords);
   else
      buffer = std::make_shared<buffer_t>(std::vector<glm::vec3>{ vertices_[0], vertices_[1], vertices_[2], vertices_[3] }, tex_coords);

   buffer->add_index_buffer(indices);
   auto object = std::make_shared<object_t>(buffer, shader_prog_, std::vector<texture_ptr_t>{ texture_ });
   object->set_drawing_style(object_t::DS_triangles);
   object->enable_face_culling(false, false);
   return object;
}
