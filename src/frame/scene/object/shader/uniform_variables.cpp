#include <GL/glew.h>
#include <glm/ext.hpp>

#include "../../../../../include/frame/scene/object/shader/shader.hpp"

using namespace LE;

shader_prog_t::uniform_variable_t::uniform_info_t::uniform_info_t() {}

inline bool compare_data(shader_prog_t::uniform_variable_t::uniform_info_t const & info, unsigned int target_type) {
   if (!info.init)
      return false;

   if (info.type != target_type)
      throw shader_program_exception_t("Bad uniform variable [" + info.name + "] type!");

   if (info.id < 0)
      throw shader_program_exception_t("Uniform [" + info.name + "] not present!");

   return true;
}

shader_prog_t::uniform_variable_t::uniform_variable_t(unsigned int prog_id, uniform_info_t const & info) :
   info_(info), prog_id_(prog_id)
{}

shader_prog_t::uniform_variable_t::~uniform_variable_t() {
   if (info_.buffer_id)
      glDeleteBuffers(1, &info_.buffer_id);
}

void shader_prog_t::uniform_variable_t::set(std::vector<glm::vec2> const & data) {
   if (!info_.init)
      return;

   glUniformBlockBinding(prog_id_, info_.index, info_.buffer_id);
   glBindBufferBase(GL_UNIFORM_BUFFER, info_.index, info_.buffer_id);
   glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec2) * data.size(), &(data[0].x), GL_DYNAMIC_DRAW);
}

void shader_prog_t::uniform_variable_t::set(std::vector<glm::vec3> const & data) {
   if (!info_.init)
      return;

   glUniformBlockBinding(prog_id_, info_.index, info_.binding_point);
   glBindBufferBase(GL_UNIFORM_BUFFER, info_.index, info_.buffer_id);
   glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec3) * data.size(), &(data[0].x), GL_DYNAMIC_DRAW);
}

void shader_prog_t::uniform_variable_t::set(std::vector<glm::vec4> const & data)
{
   if (!info_.init)
      return;

   glUniformBlockBinding(prog_id_, info_.index, info_.binding_point);
   glBindBufferBase(GL_UNIFORM_BUFFER, info_.index, info_.buffer_id);
   glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec4) * data.size(), &(data[0].x), GL_DYNAMIC_DRAW);
}

void shader_prog_t::uniform_variable_t::set(std::vector<int> const & data)
{
   if (!info_.init)
      return;

   glUniformBlockBinding(prog_id_, info_.index, info_.binding_point);
   glBindBufferBase(GL_UNIFORM_BUFFER, info_.index, info_.buffer_id);
   glBufferData(GL_UNIFORM_BUFFER, sizeof(int) * data.size(), &data[0], GL_DYNAMIC_DRAW);
}

void shader_prog_t::uniform_variable_t::set(std::vector<float> const & data)
{
   if (!info_.init)
      return;

   glUniformBlockBinding(prog_id_, info_.index, info_.binding_point);
   glBindBufferBase(GL_UNIFORM_BUFFER, info_.index, info_.buffer_id);
   glBufferData(GL_UNIFORM_BUFFER, sizeof(float) * data.size(), &data[0], GL_DYNAMIC_DRAW);
}

void shader_prog_t::uniform_variable_t::set(int const * data, size_t count)
{
   if (!compare_data(info_, GL_INT))
      return;
   glUniform1iv(info_.id, count, data);
}

void shader_prog_t::uniform_variable_t::set(unsigned int const * data, size_t count)
{
   if (!compare_data(info_, GL_UNSIGNED_INT))
      return;
   glUniform1uiv(info_.id, count, data);
}

void shader_prog_t::uniform_variable_t::set(float const * data, size_t count)
{
   if (!compare_data(info_, GL_FLOAT))
      return;
   glUniform1fv(info_.id, count, data);
}

void shader_prog_t::uniform_variable_t::set(float const data)
{
   if (!compare_data(info_, GL_FLOAT))
      return;
   glUniform1f(info_.id, data);
}

void shader_prog_t::uniform_variable_t::set(int unit)
{
   if (!info_.init)
      return;

   if (info_.type != GL_SAMPLER_2D && info_.type != GL_SAMPLER_CUBE && info_.type != GL_INT)
      throw shader_program_exception_t("Bad uniform variable [" + info_.name + "] type!");

   if (info_.id < 0)
      throw shader_program_exception_t("Uniform [" + info_.name + "] not present!");

   glUniform1i(info_.id, unit);
}

void shader_prog_t::uniform_variable_t::set(glm::vec2 const & data)
{
   if (!compare_data(info_, GL_FLOAT_VEC2))
      return;
   glUniform2fv(info_.id, 1, glm::value_ptr(data));
}

void shader_prog_t::uniform_variable_t::set(glm::vec3 const & data)
{
   if (!compare_data(info_, GL_FLOAT_VEC3))
      return;
   glUniform3fv(info_.id, 1, glm::value_ptr(data));
}

void shader_prog_t::uniform_variable_t::set(glm::vec4 const & data)
{
   if (!compare_data(info_, GL_FLOAT_VEC4))
      return;
   glUniform4fv(info_.id, 1, glm::value_ptr(data));
}

void shader_prog_t::uniform_variable_t::set(glm::mat2 const & data)
{
   if (!compare_data(info_, GL_FLOAT_MAT2))
      return;
   glUniformMatrix2fv(info_.id, 1, 0, glm::value_ptr(data));
}

void shader_prog_t::uniform_variable_t::set(glm::mat3 const & data)
{
   if (!compare_data(info_, GL_FLOAT_MAT3))
      return;
   glUniformMatrix3fv(info_.id, 1, 0, glm::value_ptr(data));
}

void shader_prog_t::uniform_variable_t::set(glm::mat4 const & data)
{
   if (!compare_data(info_, GL_FLOAT_MAT4))
      return;
   glUniformMatrix4fv(info_.id, 1, false, glm::value_ptr(data));
}

bool shader_prog_t::uniform_variable_t::init() const
{
   return info_.init;
}

void shader_prog_t::uniform_variable_t::set_init(bool is_init)
{
   info_.init = is_init;
}
