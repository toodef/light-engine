#include <light_engine.hpp>
#include <gui_common.hpp>

using namespace LE;

int main( int argc, char ** argv ) {
   examples_gui_t gui(argc, argv);

   frame_ptr_t frame = std::make_shared<frame_t>();
   frame->set_background_color(glm::vec3(0, 0, 0));
   light_engine->add_frame(frame);

   scene = std::make_shared<scene_t>();
   frame->add_scene(scene);
   user_camera = std::make_unique<user_mouse_camera_t>(scene->get_camera());
   scene->get_camera()->look_at(glm::vec3(0, 0, 0));

   shader_prog_ptr_t shader_prog = shader_prog_t::create_default();
   for (size_t i = 0; i < 1000; ++i)
      scene->add_object(builtin_objects_t::point(glm::vec3((float)rand() / RAND_MAX - 0.5, (float)rand() / RAND_MAX - 0.5, (float)rand() / RAND_MAX - 0.5), glm::vec3(0, 1, 0), shader_prog));

   gui.start();

   return 0;
}