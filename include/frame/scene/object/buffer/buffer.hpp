#pragma once

#include <vector>
#include <memory>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace LE {
   class buffer_t {
   public:
      buffer_t(std::vector<glm::vec3> const & vertices);
      buffer_t(std::vector<glm::vec3> const & vertices, glm::vec3 const& color);
      buffer_t(std::vector<glm::vec3> const& vertices, std::vector<glm::vec3> const& colors);
      buffer_t(std::vector<glm::vec3> const& vertices, std::vector<glm::vec3> const& normales, glm::vec3 const& color);
      buffer_t(std::vector<glm::vec3> const& vertices, std::vector<glm::vec3> const& normales, std::vector<glm::vec3> const& colors);
      buffer_t(std::vector<glm::vec3> const & vertices, std::vector<glm::vec2> const & tex_coords);
      buffer_t(std::vector<glm::vec3> const& vertices, glm::vec3 const& color, std::vector<glm::vec2> const& tex_coords);
      buffer_t(std::vector<glm::vec3> const& vertices, std::vector<glm::vec3> const& normales, std::vector<glm::vec2> const& tex_coords);
      buffer_t(std::vector<glm::vec3> const& vertices, std::vector<glm::vec3> const & normales, glm::vec3 const& color, std::vector<glm::vec2> const& tex_coords);
      ~buffer_t();

      void enable_vertex_attribs() const;
      void disable_vertex_attribs() const;

      void bind();
      void unbind();

      void add_index_buffer(std::vector<unsigned int> const & indices);
      bool have_idx_buffer() const;

      unsigned int vertices_number() const;
      unsigned int indices_number() const;

   private:
      enum vertex_data_components_t {VDC_positions = 1, VDC_colors_rgb = 2, VDC_colors_rgba = 4, VDC_normals = 8, VDC_texture_coords = 16};
      void calc_vertex_attribs(int data_components);

   private:
      unsigned int id_, vao_id_, vertices_number_, idx_id_, indices_number_;
      bool have_idx_buffer_;

      struct vertex_attrib_t {unsigned int index, size, pointer_position;};
      std::vector<vertex_attrib_t> vertex_attribs_;
      unsigned int vertices_stride_;
   };

   typedef std::shared_ptr<buffer_t> buffer_ptr_t;
}
