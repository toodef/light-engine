#include <iostream>

#include <light_engine.hpp>
#include <gui_common.hpp>


using namespace LE;

int main(int argc, char** argv) {
   if (argc < 2) {
      std::cerr << "Please, specify obj model path in arguments" << std::endl;
      return 1;
   }

   examples_gui_t gui(argc, argv, "Modele Loading Example");

   frame_ptr_t frame = std::make_shared<frame_t>();
   frame->set_background_color(glm::vec3(0, 0, 0));
   light_engine->add_frame(frame);

   scene_ptr_t scene = std::make_shared<scene_t>();
   frame->add_scene(scene);

   camera = scene->get_camera();
   camera->look_at(glm::vec3(0, 0, 0));
   camera->z_far(5000);
   user_camera = std::make_unique<user_wasd_camera_t>(camera);

   auto model = model_t(argv[1]);
   std::vector<object_ptr_t> models = model.compile();

   user_camera->set_camera_keyboard_sensetivity((float)glm::length(model.bbox_max_pt() - model.bbox_min_pt()) / 1000.f);
   user_camera->set_camera_wheel_sensetivity((float)glm::length(model.bbox_max_pt() - model.bbox_min_pt()) / 1000.f);

   for (auto& model : models) {
      model->set_uniforms_callback([](shader_prog_ptr_t const& shader_prog) {
         shader_prog->uniform_variable("mvp")->set(camera->model_view_projection_matrix());
         shader_prog->uniform_variable("normal_matrix")->set(camera->normal_matrix());
         shader_prog->uniform_variable("model_view")->set(camera->model_view_matrix());
         shader_prog->uniform_variable("light_position")->set(glm::vec3(0, 1, 1000));
         shader_prog->uniform_variable("light_color")->set(glm::vec3(1, 1, 1));
         shader_prog->uniform_variable("ambient_strength")->set(0.7f);
         shader_prog->uniform_variable("diffuse_strength")->set(0.3f);
         shader_prog->uniform_variable("specular_strength")->set(0.7f);
         }
      );
      model->enable_face_culling(false, false);
      scene->add_object(model);
   }

   gui.start();

   return 0;
}