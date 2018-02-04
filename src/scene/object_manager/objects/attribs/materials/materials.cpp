#include "../../../../../base.h"

using namespace SE;

////////////////////////////////////////
// material_manager_t class function
////////////////////////////////////////

material_manager_t::material_manager_t() :
   bound_material_(NULL)
{
}

material_manager_t::~material_manager_t()
{
   bound_material_ = NULL;
}

material_ptr_t material_manager_t::create_material( const string & vertex_shader_file, const string & fragment_shader_file
                                                   ,const string & geometry_shader_file )
{
   material_ptr_t new_material(new material_t(this));

   new_material->load_material_from_file(vertex_shader_file, fragment_shader_file, geometry_shader_file);

   new_material->fill_uniforms_info(new_material);

   return new_material;
}

material_ptr_t material_manager_t::create_material_from_source( const string & vertex_shader, const string & fragment_shader
                                                               ,const string & geometry_shader )
{
   material_ptr_t new_material(new material_t(this));

   new_material->load_material_from_source(vertex_shader.c_str(), fragment_shader.c_str(), geometry_shader.c_str());

   new_material->fill_uniforms_info(new_material);

   return new_material;
}

void material_manager_t::bind_material( material_t * material )
{
   if (material)
      glUseProgram(material->gl_id());
   else
      glUseProgram(0);

   bound_material_ = material;
}

material_t * material_manager_t::bound_material() const
{
   return bound_material_;
}

////////////////////////////////////////
/// uniform_t class function
////////////////////////////////////////

uniform_t::uniform_t( material_ptr_t const & mat, uniform_info_t const & info ) :
     info_(info)
   , mat_ (mat)
{
}

uniform_t::~uniform_t()
{
   if (info_.buffer_id)
      glDeleteBuffers(1, &info_.buffer_id);
}

void uniform_t::set( vector<vec2> const & data )
{
   if (!info_.init)
      return;

   glUniformBlockBinding(mat_->gl_id(), info_.index, info_.buffer_id);

   glBindBufferBase(GL_UNIFORM_BUFFER, info_.index, info_.buffer_id);

   glBufferData(GL_UNIFORM_BUFFER, sizeof(vec2) * data.size(), &(data[0].x), GL_DYNAMIC_DRAW);
}

void uniform_t::set( vector<vec3> const & data )
{
   if (!info_.init)
      return;

   glUniformBlockBinding(mat_->gl_id(), info_.index, info_.binding_point);

   glBindBufferBase(GL_UNIFORM_BUFFER, info_.index, info_.buffer_id);

   glBufferData(GL_UNIFORM_BUFFER, sizeof(vec3) * data.size(), &(data[0].x), GL_DYNAMIC_DRAW);
}

void uniform_t::set( vector<vec4> const & data )
{
   if (!info_.init)
      return;

   glUniformBlockBinding(mat_->gl_id(), info_.index, info_.binding_point);

   glBindBufferBase(GL_UNIFORM_BUFFER, info_.index, info_.buffer_id);

   glBufferData(GL_UNIFORM_BUFFER, sizeof(vec4) * data.size(), &(data[0].x), GL_DYNAMIC_DRAW);
}

void uniform_t::set( vector<int> const & data )
{
   if (!info_.init)
      return;

   glUniformBlockBinding(mat_->gl_id(), info_.index, info_.binding_point);

   glBindBufferBase(GL_UNIFORM_BUFFER, info_.index, info_.buffer_id);

   glBufferData(GL_UNIFORM_BUFFER, sizeof(int) * data.size(), &data[0], GL_DYNAMIC_DRAW);
}

void uniform_t::set( vector<float> const & data )
{
   if (!info_.init)
      return;

   glUniformBlockBinding(mat_->gl_id(), info_.index, info_.binding_point);

   glBindBufferBase(GL_UNIFORM_BUFFER, info_.index, info_.buffer_id);

   glBufferData(GL_UNIFORM_BUFFER, sizeof(float) * data.size(), &data[0], GL_DYNAMIC_DRAW);
}

void uniform_t::set( int const * data, size_t count )
{
   if (!info_.init)
      return;

   if (info_.type != GL_INT)
      cerr << "Bad uniform's type!" << endl;
   else
   {
      if (info_.id != -1)
         glUniform1iv(info_.id, count, data);
      else
         cerr << "Uniform not present" << endl;
   }
}

void uniform_t::set( unsigned int const * data , size_t count )
{
   if (!info_.init)
      return;

   if (info_.type != GL_UNSIGNED_INT)
      cerr << "Bad uniform's type!" << endl;
   else
   {
      if (info_.id != -1)
         glUniform1uiv(info_.id, count, data);
      else
         cerr << "Uniform not present" << endl;
   }

}

void uniform_t::set( float const * data, size_t count )
{
   if (!info_.init)
      return;

   if (info_.type != GL_FLOAT)
      cerr << "Bad uniform's type!" << endl;
   else
   {
      if (info_.id != -1)
         glUniform1fv(info_.id, count, data);
      else
         cerr << "Uniform not present" << endl;
   }
}

void uniform_t::set( float const data )
{
   if (!info_.init)
      return;

   if (info_.type != GL_FLOAT)
      cerr << "Bad uniform's type!" << endl;
   else
   {
      if (info_.id != -1)
         glUniform1f(info_.id, data);
      else
         cerr << "Uniform not present" << endl;
   }
}

void uniform_t::set( int unit )
{
   if (!info_.init)
      return;

   if (info_.type != GL_SAMPLER_2D && info_.type != GL_SAMPLER_CUBE && info_.type != GL_INT)
      cerr << "Bad uniform's type!" << endl;
   else
   {
      if (info_.id != -1)
         glUniform1i(info_.id, unit);
      else
         cerr << "Uniform not present" << endl;
   }
}

void uniform_t::set( vec2 const & data )
{
   if (!info_.init)
      return;

   if (info_.type != GL_FLOAT_VEC2)
      cerr << "Bad uniform's type!" << endl;
   else
   {
      if (info_.id != -1)
         glUniform2fv(info_.id, 1, value_ptr(data));
      else
         cerr << "Uniform not present" << endl;
   }
}

void uniform_t::set( vec3 const & data )
{
   if (!info_.init)
      return;

   if (info_.type != GL_FLOAT_VEC3)
      cerr << "Bad uniform's type!" << endl;
   else
   {
      if (info_.id != -1)
         glUniform3fv(info_.id, 1, value_ptr(data));
      else
         cerr << "Uniform not present" << endl;
   }
}

void uniform_t::set( vec4 const & data )
{
   if (!info_.init)
      return;

   if (info_.type != GL_FLOAT_VEC4)
      cerr << "Bad uniform's type!" << endl;
   else
   {
      if (info_.id != -1)
         glUniform4fv(info_.id, 1, value_ptr(data));
      else
         cerr << "Uniform not present" << endl;
   }
}

void uniform_t::set( mat2 const & data )
{
   if (!info_.init)
      return;

   if (info_.type != GL_FLOAT_MAT2)
      cerr << "Bad uniform's type!" << endl;
   else
   {
      if (info_.id != -1)
         glUniformMatrix2fv(info_.id, 1, 0, value_ptr(data));
      else
         cerr << "Uniform not present" << endl;
   }
}

void uniform_t::set( mat3 const & data )
{
   if (!info_.init)
      return;

   if (info_.type != GL_FLOAT_MAT3)
      cerr << "Bad uniform's type!" << endl;
   else
   {
      if (info_.id != -1)
         glUniformMatrix3fv(info_.id, 1, 0, value_ptr(data));
      else
         cerr << "Uniform not present" << endl;
   }
}

void uniform_t::set( mat4 const * data )
{
   if (!info_.init)
      return;

   if (info_.type != GL_FLOAT_MAT4)
      cerr << "Bad uniform's type!" << endl;
   else
   {
      if (info_.id != -1)
         glUniformMatrix4fv(info_.id, 1, 0, value_ptr(*data));
      else
         cerr << "Uniform not present" << endl;
   }
}

bool uniform_t::init() const
{
   return info_.init;
}

void uniform_t::set_init( bool is_init )
{
   info_.init = is_init;
}

////////////////////////////////////////
/// material_t class function
////////////////////////////////////////

material_t::material_t( material_manager_ptr_t const & manager ) :
     manager_             (manager)
   , prog_                (0)
   , is_init_             (false)
   , vertex_shader_file_  ()
   , fragment_shader_file_()
   , geometry_shader_file_()

{
}

void material_t::load_material_from_source( string const & vertex_shader_src, string const & fragment_shader_src, string const & geometry_shader_src )
{
   try
   {
      if (vertex_shader_src != "")
         vs_ = create_shader(vertex_shader_src, GL_VERTEX_SHADER);

      if (fragment_shader_src != "")
         fs_ = create_shader(fragment_shader_src, GL_FRAGMENT_SHADER);

      if (geometry_shader_src != "")
         gs_ = create_shader(geometry_shader_src, GL_GEOMETRY_SHADER);
   }
   catch(compile_exception_t & sh)
   {
      sh.print_exception();

      is_init_ = false;

      return;
   }

   try
   {
      prog_ = glCreateProgram();

      if(!prog_)
         throw create_prog_exception_t("Can't create program from default shaders");

      if (vertex_shader_src != "" && vs_->id_)
         glAttachShader(prog_, vs_->id_);

      if (fragment_shader_src != "" && fs_->id_)
         glAttachShader(prog_, fs_->id_);

      if (geometry_shader_src != "" && gs_->id_)
         glAttachShader(prog_, gs_->id_);

      glLinkProgram(prog_);

      GLint linked;
      glGetObjectParameterivARB(prog_, GL_LINK_STATUS, &linked);

      if (!linked)
      {
         char ch[1000];
         glGetProgramInfoLog(prog_, 1000, NULL, ch);

         cout << ch;
      }
   }
   catch(create_prog_exception_t & sh)
   {
      sh.print_exception();

      is_init_ = false;

      return;
   }

   is_init_ = true;
}

char * load_file( string const & file_name )
{
   filesystem::path file(file_name);

   filesystem::ifstream in_file(file, std::ios::binary);

   boost::uintmax_t size = boost::filesystem::file_size(file);

   GLchar * source(new GLchar[size + 1]);

   in_file.read(source, size);

   source[size] = 0;

   return source;
}

void material_t::load_material_from_file( string const & vertex_shader_file, string const & fragment_shader_file,
                                          const string & geometry_shader_file )
{
   if (vertex_shader_file == "" || fragment_shader_file == "")
   {
      cerr << "Incorrect shader files for material loading! Vertex shader file: '" + vertex_shader_file + "', fragment shader: '" + fragment_shader_file + "'" << endl;

      return;
   }

   scoped_array<GLchar> vs_source(load_file(vertex_shader_file));
   scoped_array<GLchar> fs_source(load_file(fragment_shader_file));

   if (geometry_shader_file == "")
   {
      load_material_from_source(vs_source.get(), fs_source.get());

      return;
   }

   scoped_array<GLchar> gs_source(load_file(geometry_shader_file));

   load_material_from_source(vs_source.get(), fs_source.get(), gs_source.get());
}

uniform_ptr_t & material_t::new_uniform( string const & name )
{
   this_ptr_ = material_weak_ptr_t(material_ptr_t(this));

   return (uniforms_[name] = uniform_ptr_t(new uniform_t(this_ptr_.lock(), uniforms_info_[name])));
}

material_t::~material_t()
{
   assert(manager_->bound_material() != this);

   if (vertex_shader_file_ != "" && vs_->id_)
   {
      glDetachShader(prog_, vs_->id_);
      glDeleteShader(vs_->id_);
   }

   if (fragment_shader_file_ != "" && fs_->id_)
   {
      glDetachShader(prog_, fs_->id_);
      glDeleteShader(fs_->id_);
   }

   if (geometry_shader_file_ != "" && gs_->id_)
   {
      glDetachShader(prog_, gs_->id_);
      glDeleteShader(gs_->id_);
   }

   glDeleteProgram(prog_);
}

GLuint material_t::gl_id() const
{
   return prog_;
}

uniform_ptr_t material_t::uniform( string const & name )
{
   uniform_ptr_t uniform = uniforms_[name];

   if (!uniform.get())
   {
      uniform = uniform_ptr_t(new uniform_t());

      uniform->set_init(false);

      uniforms_[name] = uniform;

      cerr << "Undefined uniform: '" << name << "'!" << endl;

      return uniform;
   }
   else
      return uniform;
}

void material_t::fill_uniforms_info( material_ptr_t const & material )
{
   GLint uniforms_count = 0;

   glGetProgramiv(prog_, GL_ACTIVE_UNIFORMS, &uniforms_count);

   if (uniforms_count != 0)
   {
      uniform_info_t uniform_tnp;

      this_ptr_ = material;

      char_ptr_t   name      (new GLchar[30])
                 , block_name(new GLchar[30]);

      size_t j = 0;

      for (int i = 0; i < uniforms_count; ++i)
      {
         glGetActiveUniform(prog_, i, 30, 0, &uniform_tnp.id, &uniform_tnp.type, name.get());

         uniform_tnp.init = false;

         if ((uniform_tnp.id = glGetUniformLocation(prog_, name.get())) != -1)
         {
            uniform_tnp.init = true;
            uniform_tnp.name = name.get();

            uniform_ptr_t uniform = uniform_ptr_t(new uniform_t(this_ptr_.lock(), uniform_tnp));
            uniform->set_init(true);
            uniforms_[string(uniform_tnp.name)] = uniform;
         }
         else
         {
            glGetActiveUniformBlockName(prog_, j, 30, 0, block_name.get());

            if ((uniform_tnp.index = glGetUniformBlockIndex(prog_, block_name.get())) != GL_INVALID_INDEX)
            {
               glGenBuffers(1, &uniform_tnp.buffer_id);

               uniform_tnp.init          = true;
               uniform_tnp.name          = block_name.get();
               uniform_tnp.binding_point = j++;

               uniform_ptr_t uniform = uniform_ptr_t(new uniform_t(this_ptr_.lock(), uniform_tnp));
               uniform->set_init(true);
               uniforms_[string(uniform_tnp.name)] = uniform;
            }
            else
               cerr << "Impossible to determine the position of the parameters: " << name << endl;
         }
      }
   }
}

string material_t::vs_file_name() const
{
   return vertex_shader_file_;
}

string material_t::fs_file_name() const
{
   return fragment_shader_file_;
}

string material_t::gs_file_name() const
{
   return geometry_shader_file_;
}

//////////////////////////////////
/// shader functions
//////////////////////////////////

material_t::shader_auto_ptr_t material_t::create_shader( string const & source, GLenum type )
{
   shader_auto_ptr_t shader(new shader_t(type));

   const char * src = source.c_str();

   glShaderSource(shader->id_, 1, &src, NULL);

   glCompileShader(shader->id_);

   GLint is_compilled;
   glGetObjectParameterivARB(shader->id_, GL_COMPILE_STATUS, &is_compilled);

   if (!is_compilled)
   {
      char ch[1000];
      glGetShaderInfoLog(shader->id_, 1000, NULL, ch);

      throw compile_exception_t("Troubles with compiling default shader!", ch);
   }

   return shader;
}
