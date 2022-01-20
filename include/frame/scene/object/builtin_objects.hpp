#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/vec3.hpp>
#include <vector>
#include <array>

#include "object.hpp"

namespace LE {
   class sphere_t {
   public:
      sphere_t(glm::vec3 const& center, float radius, size_t detalisation);

      void set_shader_prog(shader_prog_ptr_t const & prog);
      void set_texture(texture_ptr_t const & texture);
      void set_color(glm::vec3 const& color);

      void generate_tex_coords();
      void generate_normales();

      object_ptr_t compile() const;

   private:
      void generate_vertices_(std::vector<glm::vec3>& vertices, std::vector<unsigned int>& indices) const;

      glm::vec3 center_, color_;
      float radius_;
      size_t detalisation_;

      bool color_was_set_ = false, generate_tex_coords_ = false, generate_normales_ = false;

      shader_prog_ptr_t shader_prog_ = nullptr;
      texture_ptr_t texture_ = nullptr;
   };

   class point_t {
   public:
      point_t(glm::vec3 const& pos);

      void set_shader_prog(shader_prog_ptr_t const& prog);
      void set_texture(texture_ptr_t const& texture);
      void set_color(glm::vec3 const& color);

      object_ptr_t compile() const;

   private:
      glm::vec3 pos_, color_;
      bool color_was_set_ = false;

      shader_prog_ptr_t shader_prog_ = nullptr;
      texture_ptr_t texture_ = nullptr;
   };

   class triangle_t {
   public:
      triangle_t(std::array<glm::vec3, 3> const & vertices);

      void set_shader_prog(shader_prog_ptr_t const& prog);
      void set_texture(texture_ptr_t const& texture);
      void set_color(glm::vec3 const& color);

      object_ptr_t compile() const;

   private:
      std::array<glm::vec3, 3> vertices_;
      glm::vec3 color_;

      bool color_was_set_ = false;

      shader_prog_ptr_t shader_prog_ = nullptr;
      texture_ptr_t texture_ = nullptr;
   };

   class quad_t {
   public:
      quad_t(std::array<glm::vec3, 4> const & vertices);

      void set_shader_prog(shader_prog_ptr_t const& prog);
      void set_texture(texture_ptr_t const& texture);
      void set_color(glm::vec3 const& color);

      object_ptr_t compile() const;

   private:
      std::array<glm::vec3, 4> vertices_;
      glm::vec3 color_;

      bool color_was_set_ = false;

      shader_prog_ptr_t shader_prog_ = nullptr;
      texture_ptr_t texture_ = nullptr;
   };

   class box_t {
   public:
      box_t(glm::vec3 const& center, glm::vec3 const& up, glm::vec3 const& right, glm::vec3 const& forward);

      void set_shader_prog(shader_prog_ptr_t const& prog);
      void set_color(glm::vec3 const& color);

      object_ptr_t compile() const;

   protected:
      glm::vec3 center_, up_, forward_, right_;
      glm::vec3 color_;

      bool color_was_set_ = false;

      shader_prog_ptr_t shader_prog_ = nullptr;
   };

   class box_wireframe_t: public box_t {
   public:
      box_wireframe_t(glm::vec3 const& center, glm::vec3 const& up, glm::vec3 const& right, glm::vec3 const& forward);
      object_ptr_t compile() const;
   };

   class box_textured_t: public box_t {
   public:
      box_textured_t(glm::vec3 const& center, glm::vec3 const& up, glm::vec3 const& right, glm::vec3 const& forward);

      void set_texture(texture_ptr_t const & texture);
      object_ptr_t compile() const;

   private:
      texture_ptr_t texture_ = nullptr;
   };

   class line_t {
   public:
      line_t(glm::vec3 const& v0, glm::vec3 const& v1);

      void set_shader_prog(shader_prog_ptr_t const& prog);
      void set_texture(texture_ptr_t const& texture);
      void set_color(glm::vec3 const& color);

      object_ptr_t compile() const;

   private:
      glm::vec3 v0_, v1_;
      glm::vec3 color_;

      bool color_was_set_ = false;

      shader_prog_ptr_t shader_prog_ = nullptr;
      texture_ptr_t texture_ = nullptr;
   };

   class point_cloud_t {
   public:
      point_cloud_t(std::vector<glm::vec3> const& vertices);

      void set_color(glm::vec3 const& color);
      void set_colors(std::vector<glm::vec3> const& colors);
      void set_normales(std::vector<glm::vec3> const& normales);
      void set_shader_prog(shader_prog_ptr_t const& shader_prog);

      object_ptr_t compile() const;

   private:
      std::vector<glm::vec3> vertices_, colors_, normales_;
      shader_prog_ptr_t shader_prog_ = nullptr;
      texture_ptr_t texture_ = nullptr;
   };

   class model_t {
   public:
      model_t(std::string const& path);

      std::vector<object_ptr_t> compile();

      glm::vec3 bbox_min_pt() const;
      glm::vec3 bbox_max_pt() const;

   private:
      std::string path_;
      glm::vec3 bbox_min_pt_, bbox_max_pt_;
   };

   class builtin_objects_t {
   public:
      static object_ptr_t point(glm::vec3 const & position, glm::vec3 const & color, shader_prog_ptr_t const & shader_prog = 0, texture_ptr_t const& texture = 0);
      static object_ptr_t triangle(std::vector<glm::vec3> const & vertices, glm::vec3 const & color, shader_prog_ptr_t const & shader_prog = 0,
         texture_ptr_t const& texture = 0);
      static object_ptr_t sphere(glm::vec3 const & center, float radius, glm::vec3 const & color, unsigned int detalisation, shader_prog_ptr_t const & shader_prog = 0,
         texture_ptr_t const& texture = 0);
      static object_ptr_t quad(std::vector<glm::vec3> const & vertices, glm::vec3 const & color, shader_prog_ptr_t const & shader_prog = 0, texture_ptr_t const & texture = 0);
      static object_ptr_t box(glm::vec3 const & center, glm::vec3 const & up, glm::vec3 const & right, glm::vec3 const & forward, glm::vec3 const & color,
         shader_prog_ptr_t const & shader_prog = 0, texture_ptr_t const& texture = 0);
      static object_ptr_t point_cloud(std::vector<glm::vec3> const & positions, glm::vec3 const & color, shader_prog_ptr_t const & shader_prog = 0,
         texture_ptr_t const& texture = 0);
      static object_ptr_t line(glm::vec3 const & v0, glm::vec3 const & v1, glm::vec3 const & color, shader_prog_ptr_t const & shader_prog = 0, texture_ptr_t const& texture = 0);
   };

   struct builtin_objects_exception_t: std::exception {
      builtin_objects_exception_t(std::string const & message);
      virtual char const * what() const noexcept;

   private:
      std::string msg_;
   };
}
