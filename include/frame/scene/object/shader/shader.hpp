#pragma once

#include <string>
#include <memory>
#include <vector>
#include <map>

#include <glm/glm.hpp>

namespace LE {
   class shader_t {
   public:
      enum shader_type_t{ST_vertex = 1, ST_fragment = 2, ST_geometry = 3};

   public:
      shader_t(std::string const & source, shader_type_t shader_type);
      ~shader_t();

      unsigned int id() const;

   private:
      unsigned int id_;
   };

   typedef std::shared_ptr<shader_t> shader_ptr_t;

   struct shader_program_exception_t: std::exception {
      shader_program_exception_t(std::string const & message);
      virtual char const * what() const noexcept;

   private:
      std::string msg_;
   };

   class shader_prog_t;
   typedef std::shared_ptr<shader_prog_t> shader_prog_ptr_t;

   class shader_prog_t {
   public:
      shader_prog_t(shader_ptr_t const & v_shader = 0, shader_ptr_t const & f_shader = 0, shader_ptr_t const & g_shader = 0);
      ~shader_prog_t();

      void bind() const;
      void unbind() const;

      static shader_prog_ptr_t create_default();
      static shader_prog_ptr_t create_texture(bool draw_uv_coords = false);
      static shader_prog_ptr_t create_lightning();


      class uniform_variable_t;
      typedef std::shared_ptr<uniform_variable_t> uniform_variable_ptr_t;

      uniform_variable_ptr_t uniform_variable(std::string const & name);

   public:
      class uniform_variable_t {
      public:
         struct uniform_info_t {
            uniform_info_t();

            std::string name;
            int id;
            unsigned int buffer_id, index, binding_point;
            unsigned int type;

            bool init;
         };
      public:
         uniform_variable_t(unsigned int prog_id, uniform_info_t const & info);
         ~uniform_variable_t();

         void set(std::vector<glm::vec2> const & data);
         void set(std::vector<glm::vec3> const & data);
         void set(std::vector<glm::vec4> const & data);
         void set(std::vector<int> const & data);
         void set(std::vector<float> const & data);
         void set(int const * data, size_t count = 1);
         void set(unsigned int const * data, size_t count = 1);
         void set(float const * data, size_t count = 1);
         void set(float const data);
         void set(int unit); // for textures only
         void set(glm::vec2 const &);
         void set(glm::vec3 const &);
         void set(glm::vec4 const &);
         void set(glm::mat2 const &);
         void set(glm::mat3 const &);
         void set(glm::mat4 const &);

         bool init() const;
         void set_init(bool is_init);

      private:
         uniform_info_t info_;
         unsigned int prog_id_;
      };

   private:
      void fill_uniforms_info();

   private:
      unsigned int prog_;

      shader_ptr_t vertex_shader_, fragment_shader_, geometry_shader_;
      std::map<std::string, uniform_variable_ptr_t> uniform_variables_;
   };
}
