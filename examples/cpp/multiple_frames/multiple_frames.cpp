#include <light_engine.hpp>
#include <gui_common.hpp>

using namespace LE;

int main( int argc, char ** argv ) {
   examples_gui_t gui(argc, argv, "Multiple Frames Example");

   shader_prog_ptr_t shader_prog = shader_prog_t::create_default();

   frame_ptr_t frame_left = std::make_shared<frame_t>();
   frame_left->set_background_color(glm::vec3(0, 0, 1));
   light_engine->add_frame(frame_left, glm::vec2(0, 0), glm::vec2(0.5, 1));
   scene = std::make_shared<scene_t>();
   frame_left->add_scene(scene);
   user_camera = std::make_unique<user_mouse_camera_t>(scene->get_camera());
   scene->get_camera()->pos(glm::vec3(0, 0, -3));
   scene->get_camera()->look_at(glm::vec3(0, 0, 0));

   for (size_t i = 0; i < 100; ++i)
      scene->add_object(builtin_objects_t::point(glm::vec3((float)rand() / RAND_MAX - 0.5, (float)rand() / RAND_MAX - 0.5, (float)rand() / RAND_MAX - 0.5), glm::vec3(0, 1, 0), shader_prog));

   frame_ptr_t frame_right = std::make_shared<frame_t>();
   frame_right->set_background_color(glm::vec3(0, 1, 0));
   light_engine->add_frame(frame_right, glm::vec2(0.5, 0), glm::vec2(0.5, 1));
   scene_ptr_t scene2 = std::make_shared<scene_t>();
   frame_right->add_scene(scene2);
   scene2->set_camera(scene->get_camera());

   for (size_t i = 0; i < 100; ++i)
      scene2->add_object(builtin_objects_t::point(glm::vec3((float)rand() / RAND_MAX - 0.5, (float)rand() / RAND_MAX - 0.5, (float)rand() / RAND_MAX - 0.5), glm::vec3(0, 0, 1), shader_prog));

   gui.start();

   return 0;
}