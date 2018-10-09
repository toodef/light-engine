#include <GL/glew.h>

#include "../../../../../include/frame/scene/object/buffer/buffer.hpp"

using namespace LE;

buffer_t::buffer_t(std::vector<glm::vec3> const & vertices): vertices_number_(vertices.size()), have_idx_buffer_(false), indices_number_(0) {
   glGenBuffers(1, &id_);
   glGenVertexArrays(1, &vao_id_);

   glBindBuffer(GL_ARRAY_BUFFER, id_);
   glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   calc_vertex_attribs(VDC_positions);
}

buffer_t::buffer_t(std::vector<glm::vec3> const & vertices, glm::vec3 const & color) : 
   vertices_number_(vertices.size()), have_idx_buffer_(false), indices_number_(0)
{
   glGenBuffers(1, &id_);
   glGenVertexArrays(1, &vao_id_);

   struct data_unit {
      glm::vec3 pos, color;
   };
   
   unsigned int count = vertices.size();
   std::vector<data_unit> data(count);
   unsigned int idx = 0;
   for (unsigned int i = 0; i < vertices.size(); ++i) {
      data[i].pos = vertices[idx++];
      data[i].color = color;
   }

   glBindBuffer(GL_ARRAY_BUFFER, id_);
   glBufferData(GL_ARRAY_BUFFER, sizeof(data_unit) * vertices.size(), &data[0], GL_STATIC_DRAW);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   calc_vertex_attribs(VDC_positions | VDC_colors_rgb);
}

buffer_t::~buffer_t() {
   glDeleteBuffers(1, &id_);
   glDeleteVertexArrays(1, &vao_id_);
}

void buffer_t::calc_vertex_attribs(int data_components) {
   unsigned int last_offset = 0;
   if (data_components & VDC_positions) {
      vertex_attribs_.push_back({ last_offset, 3, last_offset });
      last_offset += 3;
   }
   if (data_components & VDC_colors_rgb) {
      vertex_attribs_.push_back({ last_offset, 3, sizeof(float) * last_offset });
      last_offset += 3;
   }
   if (data_components & VDC_colors_rgba) {
      vertex_attribs_.push_back({ last_offset, 4, sizeof(float) * last_offset });
      last_offset += 4;
   }
   if (data_components & VDC_normals) {
      vertex_attribs_.push_back({ last_offset, 3, sizeof(float) * last_offset });
      last_offset += 3;
   }
   if (data_components & VDC_texture_coords) {
      vertex_attribs_.push_back({ last_offset, 2, sizeof(float) * last_offset });
      last_offset += 2;
   }

   vertices_stride_ = last_offset * sizeof(float);
}

void buffer_t::enable_vertex_attribs() const {
   for (auto const & vertex_attrib : vertex_attribs_)
      glEnableVertexAttribArray(vertex_attrib.index);

   for (auto const & vertex_attrib : vertex_attribs_)
      glVertexAttribPointer(vertex_attrib.index, vertex_attrib.size, GL_FLOAT, GL_FALSE, vertices_stride_, (GLvoid *)vertex_attrib.pointer_position);
}

void buffer_t::disable_vertex_attribs() const {
   for (unsigned int i = vertex_attribs_.size() - 1; i > 0; --i)
      glDisableVertexAttribArray(vertex_attribs_[i].index);
}

void buffer_t::bind() {
   glBindVertexArray(vao_id_);
   glBindBuffer(GL_ARRAY_BUFFER, id_);

   if (have_idx_buffer_)
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx_id_);
}

void buffer_t::unbind() {
   if (have_idx_buffer_)
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindVertexArray(vao_id_);
}

void buffer_t::add_index_buffer(std::vector<unsigned int> const & indices) {
   glGenBuffers(1, &idx_id_);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx_id_);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   indices_number_ = indices.size();
   have_idx_buffer_ = true;
}

bool buffer_t::have_idx_buffer() const { return have_idx_buffer_; }

unsigned int buffer_t::vertices_number() const {
   return vertices_number_;
}

unsigned int buffer_t::indices_number() const {
   return indices_number_;
}

