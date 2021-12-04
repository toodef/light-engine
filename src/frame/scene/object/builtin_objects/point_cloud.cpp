#include <frame/scene/object/builtin_objects.hpp>

using namespace LE;

point_cloud_t::point_cloud_t(std::vector<glm::vec3> const& vertices) : vertices_(vertices) {}

void point_cloud_t::set_color(glm::vec3 const& color) {
   colors_.resize(vertices_.size());
   for (unsigned int i = 0; i < vertices_.size(); ++i)
      colors_[i] = color;
}

void point_cloud_t::set_colors(std::vector<glm::vec3> const& colors) {
   if (colors.size() != vertices_.size())
      throw builtin_objects_exception_t("Colors vector size doesn't equal to positions number");

   colors_.resize(vertices_.size());
   for (unsigned int i = 0; i < vertices_.size(); ++i)
      colors_[i] = colors[i];
}

void point_cloud_t::set_normales(std::vector<glm::vec3> const& normales) {
   if (normales.size() != vertices_.size())
      throw builtin_objects_exception_t("Normales vector size doesn't equal to positions number");

   normales_.resize(vertices_.size());
   for (unsigned int i = 0; i < vertices_.size(); ++i)
      normales_[i] = normales[i];
}

void point_cloud_t::set_shader_prog(shader_prog_ptr_t const& shader_prog) {
   shader_prog_ = shader_prog;
}

object_ptr_t point_cloud_t::compile() const {
   buffer_ptr_t buffer;
   if (normales_.size() > 0)
      buffer = std::make_shared<buffer_t>(vertices_, normales_, colors_);
   else
      buffer = std::make_shared<buffer_t>(vertices_, colors_);

   auto object = std::make_shared<object_t>(buffer, shader_prog_, nullptr);
   object->set_drawing_style(object_t::DS_points);
   return object;
}