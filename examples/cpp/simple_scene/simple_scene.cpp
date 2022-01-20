#include <light_engine.hpp>
#include <gui_common.hpp>

using namespace LE;

int main( int argc, char ** argv ) {
   examples_gui_t gui(argc, argv, "Simple Scene Example");

   frame_ptr_t frame = std::make_shared<frame_t>();
   frame->set_background_color(glm::vec3(0, 0, 0));
   light_engine->add_frame(frame);

   scene = std::make_shared<scene_t>();
   frame->add_scene(scene);
   user_camera = std::make_unique<user_mouse_camera_t>(scene->get_camera());
   scene->get_camera()->look_at(glm::vec3(0, 0, 0));

   shader_prog_ptr_t shader_prog = shader_prog_t::create_default();
   for (size_t i = 0; i < 1000; ++i) {
      point_t point(glm::vec3((float)rand() / RAND_MAX - 0.5, (float)rand() / RAND_MAX - 0.5, (float)rand() / RAND_MAX - 0.5));
      point.set_shader_prog(shader_prog);
      point.set_color(glm::vec3(0, 0, 1));
      object_ptr_t object = point.compile();
      object->set_points_size(3);
      scene->add_object(object);
   }

   gui.start();

   return 0;
}