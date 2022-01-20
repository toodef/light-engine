#include <iostream>

#include <light_engine.hpp>
#include <gui_common.hpp>


using namespace LE;

int main(int argc, char** argv) {
   if (argc < 2) {
      std::cerr << "Please, specify image path in arguments" << std::endl;
      return 1;
   }

   examples_gui_t gui(argc, argv, "Texture Example");

   frame_ptr_t frame = std::make_shared<frame_t>();
   frame->set_background_color(glm::vec3(0, 0, 0));
   light_engine->add_frame(frame);

   scene_ptr_t scene = std::make_shared<scene_t>();
   frame->add_scene(scene);
   user_camera = std::make_unique<user_mouse_camera_t>(scene->get_camera());
   scene->get_camera()->look_at(glm::vec3(0, 0, 0));

   image_ptr_t image = std::make_shared<image_t>(argv[1]);
   texture_ptr_t texture = std::make_shared<texture_t>(image);
   
   shader_prog_ptr_t shader_prog = shader_prog_t::create_texture();
   sphere_t sphere(glm::vec3(0, 0, 0), 0.5f, 5);
   sphere.set_shader_prog(shader_prog);
   sphere.set_texture(texture);
   sphere.generate_tex_coords();
   scene->add_object(sphere.compile());

   gui.start();

   return 0;
}