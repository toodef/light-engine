#include <light_engine.hpp>
#include <gui_common.hpp>


using namespace LE;

static shader_prog_ptr_t create_shader_prog() {
   const std::string v_shader_src = "#version 420\n"
      "layout ( location = 0 ) in vec3 pos;\n"
      "layout ( location = 3 ) in vec3 col;\n"
      "layout ( location = 6 ) in vec2 tex;\n"
      
      "out vec3 out_color;\n"
      "out vec2 out_tex_coord;\n"

      "uniform mat4 mvp;\n"

      "void main()\n"
      "{\n"
      "   gl_Position = mvp * vec4(pos, 1.0f);\n"
      "   out_color = vec3(tex[0], tex[1], 1.0f);"
      "   out_tex_coord = tex;\n"
      "}";

   const std::string f_shader_src = "#version 420\n"
      "in vec3 out_color;\n"
      "in vec2 out_tex_coord;\n"

      "out vec4 color;\n"

      "uniform sampler2D image;\n"

      "void main()\n"
      "{\n"
      "   color =  texture(image, out_tex_coord);\n"
      "}";

   shader_ptr_t vertex_shader = std::make_shared<shader_t>(v_shader_src, shader_t::ST_vertex),
      fragment_shader = std::make_shared<shader_t>(f_shader_src, shader_t::ST_fragment);
   return std::make_shared<shader_prog_t>(vertex_shader, fragment_shader);
}


int main(int argc, char** argv) {
   examples_gui_t gui(argc, argv, "Texture Example");

   frame_ptr_t frame = std::make_shared<frame_t>();
   frame->set_background_color(glm::vec3(0, 0, 0));
   light_engine->add_frame(frame);

   scene_ptr_t scene = std::make_shared<scene_t>();
   frame->add_scene(scene);
   user_camera = std::make_unique<user_mouse_camera_t>(scene->get_camera());
   scene->get_camera()->look_at(glm::vec3(0, 0, 0));

   image_ptr_t image = std::make_shared<image_t>("img.jpg");
   texture_ptr_t texture = std::make_shared<texture_t>(image);
   
   shader_prog_ptr_t shader_prog = create_shader_prog();
   scene->add_object(builtin_objects_t::quad({ glm::vec3(-0.5, -0.5, 0), glm::vec3(-0.5, 0.5, 0), glm::vec3(0.5, 0.5, 0), glm::vec3(0.5, -0.5, 0) }, glm::vec3(0, 1, 0), shader_prog, texture));

   gui.start();

   return 0;
}