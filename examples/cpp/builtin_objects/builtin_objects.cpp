#include <light_engine.hpp>
#include <gui_common.hpp>

using namespace LE;

void generate_points(glm::vec3 const & min_pt, glm::vec3 const& max_pt) {
   shader_prog_ptr_t shader_prog = shader_prog_t::create_default();
   for (size_t i = 0; i < 10; ++i) {
      point_t point(min_pt + (max_pt - min_pt) * glm::vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX));
      point.set_shader_prog(shader_prog);
      point.set_color(glm::vec3(0, 1, 0));
      object_ptr_t object = point.compile();
      object->set_points_size(i + 1);
      scene->add_object(object);
   }
}

void generate_triangles(glm::vec3 const& min_pt, glm::vec3 const& max_pt) {
   shader_prog_ptr_t shader_prog = shader_prog_t::create_default();
   std::array<glm::vec3, 3> triangle_vertices;
   for (size_t i = 0; i < 2; ++i) {
      for (auto & vertex : triangle_vertices)
         vertex = glm::vec3(min_pt + (max_pt - min_pt) * glm::vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX));
      triangle_t triangle(triangle_vertices);
      triangle.set_shader_prog(shader_prog);
      triangle.set_color(glm::vec3(0.5, 0, 0.5));
      object_ptr_t object = triangle.compile();
      scene->add_object(object);
   }
}

void generate_spheres(glm::vec3 const& min_pt, glm::vec3 const& max_pt) {
   shader_prog_ptr_t shader_prog = shader_prog_t::create_default();

   sphere_t regular_sphere(min_pt + (max_pt - min_pt) * glm::vec3(0.33, 0.5, 0.33), 0.5, 1);
   regular_sphere.set_shader_prog(shader_prog);
   regular_sphere.set_color(glm::vec3(0, 1, 0));
   scene->add_object(regular_sphere.compile());

   {
      sphere_t detailed_sphere(min_pt + (max_pt - min_pt) * glm::vec3(0.66, 0.5, 0.33), 0.5, 3);
      detailed_sphere.set_shader_prog(shader_prog);
      detailed_sphere.set_color(glm::vec3(0, 1, 0));
      object_ptr_t object = detailed_sphere.compile();
      object->set_polygon_mode(object_t::PM_line);
      scene->add_object(object);
   }
   {
      sphere_t lighted_sphere(min_pt + (max_pt - min_pt) * glm::vec3(0.33, 0.5, 0.66), 0.5, 3);
      lighted_sphere.set_shader_prog(shader_prog_t::create_lightning());
      lighted_sphere.set_color(glm::vec3(0, 1, 0));
      lighted_sphere.generate_normales();
      object_ptr_t object = lighted_sphere.compile();
      object->set_uniforms_callback([] (shader_prog_ptr_t const& shader_prog) {
            shader_prog->uniform_variable("mvp")->set(camera->model_view_projection_matrix());
            shader_prog->uniform_variable("normal_matrix")->set(camera->normal_matrix());
            shader_prog->uniform_variable("model_view")->set(camera->model_view_matrix());
            shader_prog->uniform_variable("light_position")->set(glm::vec3(0, 1, 10));
            shader_prog->uniform_variable("light_color")->set(glm::vec3(1, 1, 1));
            shader_prog->uniform_variable("ambient_strength")->set(0.4f);
            shader_prog->uniform_variable("diffuse_strength")->set(0.3f);
            shader_prog->uniform_variable("specular_strength")->set(0.7f);
         }
      );
      scene->add_object(object);
   }
   {
      sphere_t detailed_sphere(min_pt + (max_pt - min_pt) * glm::vec3(0.66, 0.5, 0.66), 0.5, 5);
      detailed_sphere.set_shader_prog(shader_prog_t::create_texture());
      detailed_sphere.set_texture(std::make_shared<texture_t>(image_t::generate_chess(32, 32)));
      detailed_sphere.generate_tex_coords();
      object_ptr_t object = detailed_sphere.compile();
      scene->add_object(object);
   }
}

void generate_quads(glm::vec3 const& min_pt, glm::vec3 const& max_pt) {
   shader_prog_ptr_t shader_prog = shader_prog_t::create_default();

   float half_z = ((min_pt + max_pt) / 2.f).z;
   std::array<glm::vec3, 4> quad_vertices{
      glm::vec3(min_pt.x, min_pt.y, half_z),
      glm::vec3(min_pt.x, max_pt.y, half_z),
      glm::vec3(max_pt.x, max_pt.y, half_z),
      glm::vec3(max_pt.x, min_pt.y, half_z)
   };
   
   quad_t quad(quad_vertices);
   quad.set_shader_prog(shader_prog_t::create_texture());
   quad.set_texture(std::make_shared<texture_t>(image_t::generate_chess(6, 6)));
   object_ptr_t object = quad.compile();
   scene->add_object(object);
}

void generate_box(glm::vec3 const& min_pt, glm::vec3 const& max_pt) {
   shader_prog_ptr_t shader_prog = shader_prog_t::create_default();

   //box_t box((min_pt + max_pt) / 2.f, glm::vec3(0, 0.5, 0), glm::vec3(0.5, 0, 0), glm::vec3(0, 0, 0.5));
   box_t box(glm::vec3(0, 0, 0), glm::vec3(0, 0.5, 0), glm::vec3(0.5, 0, 0), glm::vec3(0, 0, 0.5));
   box.set_shader_prog(shader_prog);
   box.set_color(glm::vec3(1, 1, 0));
   object_ptr_t object = box.compile();
   scene->add_object(object);
}

void generate_lines(glm::vec3 const& min_pt, glm::vec3 const& max_pt) {
   shader_prog_ptr_t shader_prog = shader_prog_t::create_default();

   for (unsigned int i = 0; i < 50; ++i) {
      line_t line(min_pt + (max_pt - min_pt) * glm::vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX),
                  min_pt + (max_pt - min_pt) * glm::vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX));
      line.set_shader_prog(shader_prog);
      line.set_color(glm::vec3(0.5, 0.5, 1));
      object_ptr_t object = line.compile();
      scene->add_object(object);
   }
}

void generate_point_cloud(glm::vec3 const& min_pt, glm::vec3 const& max_pt) {
   shader_prog_ptr_t shader_prog = shader_prog_t::create_default();
   const size_t points_num = 10000;
   std::vector<glm::vec3> points(points_num);

   for (size_t i = 0; i < points_num; ++i)
      points[i] = min_pt + (max_pt - min_pt) * glm::vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);

   point_cloud_t point_cloud = point_cloud_t(points);
   point_cloud.set_shader_prog(shader_prog);
   point_cloud.set_color(glm::vec3(0.5, 1, 0.5));
   object_ptr_t object = point_cloud.compile();
   object->set_points_size(2);
   scene->add_object(object);
}

void generate_billboard(glm::vec3 const& min_pt, glm::vec3 const& max_pt) {
   const float half_z = ((min_pt + max_pt) / 2.f).z;
   std::array<glm::vec3, 4> quad_vertices{
      glm::vec3(min_pt.x, min_pt.y, half_z),
      glm::vec3(min_pt.x, max_pt.y, half_z),
      glm::vec3(max_pt.x, max_pt.y, half_z),
      glm::vec3(max_pt.x, min_pt.y, half_z)
      //glm::vec3(min_pt.x, min_pt.y, 0),
      //glm::vec3(min_pt.x, max_pt.y, 0),
      //glm::vec3(max_pt.x, max_pt.y, 0),
      //glm::vec3(max_pt.x, min_pt.y, 0)
   };

   quad_t quad(quad_vertices);
   quad.set_shader_prog(shader_prog_t::create_billboard());
   quad.set_color(glm::vec3(1, 0, 0));
   object_ptr_t object = quad.compile();
   object->set_uniforms_callback([](shader_prog_ptr_t const& shader_prog) {
      shader_prog->uniform_variable("billboard_matrix")->set(camera->billboard_matrix(camera->pos()));
      //shader_prog->uniform_variable("model_view")->set(camera->model_view_matrix());
      //shader_prog->uniform_variable("project")->set(camera->projection_matrix());
      //shader_prog->uniform_variable("offset")->set(glm::vec3(0, 0, 13.33333) - camera->pos());
      }
   );
   scene->add_object(object);
}

void draw_mesh(glm::vec3 const & min_pt, glm::vec3 const & max_pt, glm::vec3 const & cells_num) {
   const glm::vec3 cell_size = (max_pt - min_pt) / cells_num;
   shader_prog_ptr_t shader_prog = shader_prog_t::create_default();

   for (size_t i = 0; i < cells_num.x; ++i)
      for (size_t j = 0; j < cells_num.y; ++j)
         for (size_t k = 0; k < cells_num.z; ++k) {
            box_wireframe_t box(min_pt + cell_size * glm::vec3(i, j, k) + cell_size / 2.f,
                      0.5f * cell_size * glm::vec3(0, 1, 0), 0.5f * cell_size * glm::vec3(1, 0, 0), 0.5f * cell_size * glm::vec3(0, 0, 1));
            box.set_shader_prog(shader_prog);
            box.set_color(glm::vec3(1, 1, 1));
            object_ptr_t object = box.compile();
            scene->add_object(object);
         }
}

int main( int argc, char ** argv ) {
   examples_gui_t gui(argc, argv, "Builtin Objects Example");

   frame_ptr_t frame = std::make_shared<frame_t>();
   frame->set_background_color(glm::vec3(0, 0, 0));
   light_engine->add_frame(frame);

   scene = std::make_shared<scene_t>();
   frame->add_scene(scene);
   camera = scene->get_camera();
   user_camera = std::make_unique<user_wasd_camera_t>(camera);
   user_camera->set_camera_wheel_sensetivity(2);
   user_camera->set_camera_keyboard_sensetivity(0.01f);
   //scene->get_camera()->pos(glm::vec3(5, 4, -5));
   //scene->get_camera()->look_at(glm::vec3(5, 0, 0));

   const glm::vec3 cells_num = glm::vec3(3, 1, 3);
   const glm::vec3 min_pt = glm::vec3(-5.f, -10.f / 3, 5.f), max_pt = glm::vec3(5.f, 0.f, 15.f);
   const glm::vec3 cell_size = (max_pt - min_pt) / cells_num;

   draw_mesh(min_pt, max_pt, cells_num);

   unsigned int cur_cell_x = 0, cur_cell_z = 0;
   generate_points(min_pt + glm::vec3(cur_cell_x, 0, cur_cell_z) * cell_size, min_pt + glm::vec3(cur_cell_x + 1, 1, cur_cell_z + 1) * cell_size);

   cur_cell_x = 1; cur_cell_z = 0;
   generate_triangles(min_pt + glm::vec3(cur_cell_x, 0, cur_cell_z) * cell_size, min_pt + glm::vec3(cur_cell_x + 1, 1, cur_cell_z + 1) * cell_size);

   cur_cell_x = 2; cur_cell_z = 0;
   generate_spheres(min_pt + glm::vec3(cur_cell_x, 0, cur_cell_z) * cell_size, min_pt + glm::vec3(cur_cell_x + 1, 1, cur_cell_z + 1) * cell_size);

   cur_cell_x = 0; cur_cell_z = 1;
   generate_quads(min_pt + glm::vec3(cur_cell_x, 0, cur_cell_z) * cell_size, min_pt + glm::vec3(cur_cell_x + 1, 1, cur_cell_z + 1) * cell_size);

   cur_cell_x = 1; cur_cell_z = 1;
   generate_box(min_pt + glm::vec3(cur_cell_x, 0, cur_cell_z) * cell_size, min_pt + glm::vec3(cur_cell_x + 1, 1, cur_cell_z + 1) * cell_size);

   cur_cell_x = 2; cur_cell_z = 1;
   generate_lines(min_pt + glm::vec3(cur_cell_x, 0, cur_cell_z) * cell_size, min_pt + glm::vec3(cur_cell_x + 1, 1, cur_cell_z + 1) * cell_size);

   cur_cell_x = 0; cur_cell_z = 2;
   generate_point_cloud(min_pt + glm::vec3(cur_cell_x, 0, cur_cell_z) * cell_size, min_pt + glm::vec3(cur_cell_x + 1, 1, cur_cell_z + 1) * cell_size);

   cur_cell_x = 1; cur_cell_z = 2;
   generate_billboard(min_pt + glm::vec3(cur_cell_x, 0, cur_cell_z) * cell_size, min_pt + glm::vec3(cur_cell_x + 1, 1, cur_cell_z + 1) * cell_size);

   gui.start();

   return 0;
}