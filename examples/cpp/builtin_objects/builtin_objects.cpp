#include <light_engine.hpp>
#include <gui_common.hpp>

using namespace LE;

int main( int argc, char ** argv ) {
   examples_gui_t gui(argc, argv, "Builtin Objects Example");

   frame_ptr_t frame = std::make_shared<frame_t>();
   frame->set_background_color(glm::vec3(0, 0, 0));
   light_engine->add_frame(frame);

   scene = std::make_shared<scene_t>();
   frame->add_scene(scene);
   user_camera = std::make_unique<user_mouse_camera_t>(scene->get_camera());
   scene->get_camera()->pos(glm::vec3(0, 0, -3));
   scene->get_camera()->look_at(glm::vec3(0, 0, 0));

   const unsigned int cells_num_x = 3, cells_num_y = 2;
   unsigned int cur_cell_x = 0, cur_cell_y = 0;

   float cur_cell_min_x = cur_cell_x - (float)cells_num_x / 2, cur_cell_min_y = cur_cell_y - (float)cells_num_y / 2;

   shader_prog_ptr_t shader_prog = shader_prog_t::create_default();
   for (size_t i = 0; i < 10; ++i)
      scene->add_object(builtin_objects_t::point(glm::vec3((float)rand() / RAND_MAX + cur_cell_min_x, (float)rand() / RAND_MAX + cur_cell_min_y, (float)rand() / RAND_MAX - 0.5), glm::vec3(0, 1, 0), shader_prog));

   cur_cell_x = 1;
   cur_cell_y = 0;
   cur_cell_min_x = cur_cell_x - (float)cells_num_x / 2;
   cur_cell_min_y = cur_cell_y - (float)cells_num_y / 2;
   std::vector<glm::vec3> triangle_vertices(3);
   for (size_t i = 0; i < 2; ++i) {
      for (auto & vertex : triangle_vertices)
         vertex = glm::vec3((float)rand() / RAND_MAX + cur_cell_min_x, (float)rand() / RAND_MAX + cur_cell_min_y, (float)rand() / RAND_MAX - 0.5);
      scene->add_object(builtin_objects_t::triangle(triangle_vertices, glm::vec3(1, 0, 0), shader_prog));
   }

   cur_cell_x = 0;
   cur_cell_y = 1;
   cur_cell_min_x = cur_cell_x - (float)cells_num_x / 2;
   cur_cell_min_y = cur_cell_y - (float)cells_num_y / 2;
   scene->add_object(builtin_objects_t::sphere(glm::vec3(cur_cell_min_x + 0.5, cur_cell_min_y + 0.5, 0.5), 0.5, glm::vec3(1, 1, 1), 2, shader_prog));

   cur_cell_x = 1;
   cur_cell_y = 1;
   cur_cell_min_x = cur_cell_x - (float)cells_num_x / 2;
   cur_cell_min_y = cur_cell_y - (float)cells_num_y / 2;
   std::vector<glm::vec3> quad_vertices(4);

   quad_vertices[0] = glm::vec3(cur_cell_min_x, cur_cell_min_y, 0);
   quad_vertices[1] = glm::vec3(cur_cell_min_x, cur_cell_min_y + 1, 0);
   quad_vertices[2] = glm::vec3(cur_cell_min_x + 1, cur_cell_min_y + 1, 0);
   quad_vertices[3] = glm::vec3(cur_cell_min_x + 1, cur_cell_min_y, 0);
   scene->add_object(builtin_objects_t::quad(quad_vertices, glm::vec3(0, 0, 1), shader_prog));

   cur_cell_x = 2;
   cur_cell_y = 0;
   cur_cell_min_x = cur_cell_x - (float)cells_num_x / 2;
   cur_cell_min_y = cur_cell_y - (float)cells_num_y / 2;
   scene->add_object(builtin_objects_t::box(glm::vec3(cur_cell_min_x + 0.5, cur_cell_min_y + 0.5, 0), glm::vec3(0, 0.5, 0), glm::vec3(0.5, 0, 0), glm::vec3(0, 0, 0.5), glm::vec3(1, 0, 1), shader_prog));

   cur_cell_x = 2;
   cur_cell_y = 1;
   cur_cell_min_x = cur_cell_x - (float)cells_num_x / 2;
   cur_cell_min_y = cur_cell_y - (float)cells_num_y / 2;
   for (unsigned int i = 0; i < 5; ++i)
      scene->add_object(builtin_objects_t::line(glm::vec3((float)rand() / RAND_MAX + cur_cell_min_x, (float)rand() / RAND_MAX + cur_cell_min_y, (float)rand() / RAND_MAX - 0.5), glm::vec3((float)rand() / RAND_MAX + cur_cell_min_x, (float)rand() / RAND_MAX + cur_cell_min_y, (float)rand() / RAND_MAX - 0.5), glm::vec3(1, 1, 0), shader_prog));

   gui.start();

   return 0;
}