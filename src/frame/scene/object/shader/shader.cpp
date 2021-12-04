#include <GL/glew.h>

#include "../../../../../include/frame/scene/object/shader/shader.hpp"

using namespace LE;

//////////////////////////////////////////
// shader_t
//////////////////////////////////////////

shader_t::shader_t(std::string const & source, shader_type_t shader_type) {
   GLenum gl_type = GL_VERTEX_SHADER;
   
   switch (shader_type) {
   case ST_vertex:
      gl_type = GL_VERTEX_SHADER;
      break;
   case ST_fragment:
      gl_type = GL_FRAGMENT_SHADER;
      break;
   case ST_geometry:
      gl_type = GL_GEOMETRY_SHADER;
      break;
   }
   id_ = glCreateShader(gl_type);
   const char * src = source.c_str();
   glShaderSource(id_, 1, &src, NULL);
   glCompileShader(id_);

   GLint is_compilled;
   glGetObjectParameterivARB(id_, GL_COMPILE_STATUS, &is_compilled);
   if (!is_compilled){
      char ch[1000];
      glGetShaderInfoLog(id_, 1000, NULL, ch);

      throw shader_program_exception_t("Troubles with compiling default shader! Message: " + std::string(ch));
   }
}

shader_t::~shader_t() {
   glDeleteShader(id_);
}

unsigned int shader_t::id() const {
   return id_;
}

//////////////////////////////////////////
// shader_prog_t
//////////////////////////////////////////

shader_program_exception_t::shader_program_exception_t(std::string const & message) : msg_(message) {}
char const * shader_program_exception_t::what() const noexcept { return msg_.c_str(); }

shader_prog_t::shader_prog_t(shader_ptr_t const & v_shader, shader_ptr_t const & f_shader, shader_ptr_t const & g_shader):
   vertex_shader_(v_shader), fragment_shader_(f_shader), geometry_shader_(g_shader)
{
   if (!v_shader && !f_shader && !g_shader)
      throw shader_program_exception_t("All shaders not initialized. Exception from shader program creation.");

   prog_ = glCreateProgram();
   if (v_shader)
      glAttachShader(prog_, v_shader->id());
   if (f_shader)
      glAttachShader(prog_, f_shader->id());
   if (g_shader)
      glAttachShader(prog_, g_shader->id());

   glLinkProgram(prog_);
   GLint linked;
   glGetObjectParameterivARB(prog_, GL_LINK_STATUS, &linked);
   if (!linked) {
      char ch[1000];
      glGetProgramInfoLog(prog_, 1000, NULL, ch);

      throw shader_program_exception_t("Error linked shader programm. Info: [" + std::string(ch) + "]");
   }

   fill_uniforms_info();
}

shader_prog_t::~shader_prog_t() {
   if (vertex_shader_)
      glDetachShader(prog_, vertex_shader_->id());
   if (fragment_shader_)
      glDetachShader(prog_, fragment_shader_->id());
   if (geometry_shader_)
      glDetachShader(prog_, geometry_shader_->id());

   glDeleteProgram(prog_);
}

void shader_prog_t::bind() const { glUseProgram(prog_); }
void shader_prog_t::unbind() const { glUseProgram(0); };

shader_prog_ptr_t shader_prog_t::create_default() {
   const std::string vertex_src = "#version 420\n"
      "layout ( location = 0 )in vec3 pos;\n"
      "layout ( location = 3 )in vec3 col;\n"
      "out vec3 out_col;\n"
      "uniform mat4 mvp;\n"
      "void main()\n"
      "{\n"
      "   out_col = col;\n"
      "   gl_Position = mvp * vec4(pos, 1.0f);\n"
      "}",
      fragment_src = "#version 420\n"
      "in vec3 out_col;\n"
      "layout ( location = 0 ) out vec4 color;\n"
      "void main()\n"
      "{\n"
      "   color = vec4(out_col, 1.0f);\n"
      "}";

   return std::make_shared<shader_prog_t>(std::make_shared<shader_t>(vertex_src, shader_t::ST_vertex),
                                          std::make_shared<shader_t>(fragment_src, shader_t::ST_fragment));
}

shader_prog_ptr_t shader_prog_t::create_texture(bool draw_uv_coords) {
   const std::string v_shader_src = "#version 420\n"
      "layout ( location = 0 ) in vec3 pos;\n"
      "layout ( location = 3 ) in vec2 tex;\n"

      "out vec3 out_color;\n"
      "out vec2 out_tex_coord;\n"

      "uniform mat4 mvp;\n"

      "void main()\n"
      "{\n"
      "   gl_Position = mvp * vec4(pos, 1.0f);\n"
      "   out_color = vec3(tex[0], tex[1], 1.0f);"
      "   out_tex_coord = tex;\n"
      "}";

   std::string f_shader_src = "#version 420\n"
      "in vec3 out_color;\n"
      "in vec2 out_tex_coord;\n"

      "out vec4 color;\n"

      "uniform sampler2D image;\n"

      "void main()\n"
      "{\n";
      if (draw_uv_coords)
         f_shader_src = f_shader_src + std::string("   color = vec4(out_tex_coord, 1.f, 1.f);\n}");
      else
         f_shader_src = f_shader_src + std::string("   color = texture(image, out_tex_coord);\n}");

   shader_ptr_t vertex_shader = std::make_shared<shader_t>(v_shader_src, shader_t::ST_vertex),
      fragment_shader = std::make_shared<shader_t>(f_shader_src, shader_t::ST_fragment);
   return std::make_shared<shader_prog_t>(vertex_shader, fragment_shader);
}

shader_prog_ptr_t shader_prog_t::create_lightning() {
   const std::string v_shader_src = "#version 420\n"
      "layout ( location = 0 )in vec3 pos;\n"
      "layout ( location = 3 )in vec3 col;\n"
      "layout ( location = 6 )in vec3 normal;\n"

      "out vec3 vert_pos;\n"
      "out vec3 vert_col;\n"
      "out vec3 vert_normal;\n"
      "out vec3 light_pos;\n"

      "uniform mat4 mvp;\n"
      "uniform mat4 normal_matrix;\n"
      "uniform mat4 model_view;\n"
      "uniform vec3 light_position;\n"

      "void main()\n"
      "{\n"
      "   vert_col = col;\n"
      "   vert_pos = vec3(model_view * vec4(pos, 1));\n"
      "   vert_normal = mat3(normal_matrix) * normal;\n"
      "   light_pos = vec3(model_view * vec4(light_position, 1));\n"
      "   gl_Position = mvp * vec4(pos, 1.0f);\n"
      "}";

   const std::string f_shader_src = "#version 420\n"
      "in vec3 vert_pos;\n"
      "in vec3 vert_col;\n"
      "in vec3 vert_normal;\n"
      "in vec3 light_pos;\n"

      "layout ( location = 0 ) out vec4 frag_color;\n"

      "uniform vec3 light_color;\n"
      "uniform float ambient_strength;\n"
      "uniform float diffuse_strength;\n"
      "uniform float specular_strength;\n"

      "void main()\n"
      "{\n"
      // ambient
      "   vec3 ambient = ambient_strength * light_color;\n"

      // diffuse
      "   vec3 norm = normalize(vert_normal);\n"
      "   vec3 lightDir = normalize(light_pos - vert_pos);\n"
      "   float diff = max(dot(norm, lightDir), 0.0);\n"
      "   vec3 diffuse = diffuse_strength * diff * light_color;\n"

      // specular
      "   vec3 viewDir = normalize(-vert_pos);\n"
      "   vec3 reflectDir = reflect(-lightDir, norm);\n"
      "   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);\n"
      "   vec3 specular = specular_strength * spec * light_color;\n"

      "   vec3 result = (ambient + diffuse + specular) * vert_col; \n"
      "   frag_color = vec4(result, 1.0); \n"
      "}";

   shader_ptr_t vertex_shader = std::make_shared<shader_t>(v_shader_src, shader_t::ST_vertex),
      fragment_shader = std::make_shared<shader_t>(f_shader_src, shader_t::ST_fragment);
   return std::make_shared<shader_prog_t>(vertex_shader, fragment_shader);
}

shader_prog_t::uniform_variable_ptr_t shader_prog_t::uniform_variable(std::string const & name) {
   return uniform_variables_[name];
}

// TODO: looks like shit. Need to optimize
void shader_prog_t::fill_uniforms_info() {
   GLint uniforms_count = 0;

   glGetProgramiv(prog_, GL_ACTIVE_UNIFORMS, &uniforms_count);

   if (uniforms_count != 0) {
      uniform_variable_t::uniform_info_t uniform_tmp;

      char * name = new GLchar[30], * block_name = new GLchar[30];
      size_t j = 0;

      for (int i = 0; i < uniforms_count; ++i) {
         glGetActiveUniform(prog_, i, 30, 0, &uniform_tmp.id, &uniform_tmp.type, name);

         uniform_tmp.init = false;

         if ((uniform_tmp.id = glGetUniformLocation(prog_, name)) != -1) {
            uniform_tmp.init = true;
            uniform_tmp.name = name;

            uniform_variable_ptr_t uniform = uniform_variable_ptr_t(new uniform_variable_t(prog_, uniform_tmp));
            uniform->set_init(true);
            uniform_variables_[uniform_tmp.name] = uniform;
         }
         else {
            glGetActiveUniformBlockName(prog_, j, 30, 0, block_name);

            if ((uniform_tmp.index = glGetUniformBlockIndex(prog_, block_name)) != GL_INVALID_INDEX) {
               glGenBuffers(1, &uniform_tmp.buffer_id);

               uniform_tmp.init = true;
               uniform_tmp.name = block_name;
               uniform_tmp.binding_point = j++;

               uniform_variable_ptr_t uniform = std::make_shared<uniform_variable_t>(prog_, uniform_tmp);
               uniform->set_init(true);
               uniform_variables_[uniform_tmp.name] = uniform;
            }
            else
               throw shader_program_exception_t("Impossible to determine the position of the parameters: " + std::string(name));
         }
      }
   }
}
