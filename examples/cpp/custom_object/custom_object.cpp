#include <light_engine.hpp>
#include <gui_common.hpp>

using namespace LE;

class le_logo_t : public object_t {
public:
   le_logo_t(camera_ptr_t const & camera) : object_t(create_buffer(), shader_prog_t::create_lightning()), camera_(camera) {
      set_drawing_style(object_t::DS_triangles);
   }

   inline static buffer_ptr_t create_buffer() {
      static const glm::vec3 color(0, 1, 0);

      glm::vec3 l_offset(-0.5, -0.5, 0);
      std::vector<glm::vec3> vertices = {
         // E letter
         glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), glm::vec3(1, 0.4, 0), glm::vec3(0.4, 0.4, 0),
         glm::vec3(0.4, 1 - 0.2, 0), glm::vec3(0.7, 1 - 0.2, 0), glm::vec3(0.7, 1 + 0.2, 0), glm::vec3(0.4, 1 + 0.2, 0),
         glm::vec3(0.4, 1.6, 0), glm::vec3(1, 1.6, 0), glm::vec3(1, 2, 0), glm::vec3(0, 2, 0),

         // L letter
         glm::vec3(0, 0, 0) + l_offset, glm::vec3(1, 0, 0) + l_offset, glm::vec3(1, 0.4, 0) + l_offset,
         glm::vec3(0.4, 0.4, 0) + l_offset, glm::vec3(0.4, 2, 0) + l_offset,
         glm::vec3(0, 2, 0) + l_offset
      };

      std::vector<unsigned int> indices {
         // E letter
         0, 1, 3,
         1, 2, 3,
         0, 3, 8,
         4, 5, 6,
         4, 6, 7,
         8, 9, 10,
         8, 10, 11,
         0, 8, 11,
         // L letter
         12 + 0, 12 + 1, 12 + 3,
         12 + 1, 12 + 2, 12 + 3,
         12 + 0, 12 + 3, 12 + 4,
         12 + 0, 12 + 4, 12 + 5
      };

      std::vector<glm::vec3> normales(vertices.size());
      for (auto & norm: normales)
         norm = glm::vec3(0, 0, 1);

      buffer_ptr_t buffer = std::make_shared<buffer_t>(vertices, normales, color);
      buffer->add_index_buffer(indices);
      return buffer;
   }

private:
   inline void draw_buffer() const {
      shader_prog_->uniform_variable("mvp")->set(camera_->model_view_projection_matrix());
      shader_prog_->uniform_variable("normal_matrix")->set(camera_->normal_matrix());
      shader_prog_->uniform_variable("model_view")->set(camera_->model_view_matrix());
      shader_prog_->uniform_variable("light_position")->set(glm::vec3(0, 1, 10));
      shader_prog_->uniform_variable("light_color")->set(glm::vec3(1, 1, 1));
      shader_prog_->uniform_variable("ambient_strength")->set(0.2f);
      shader_prog_->uniform_variable("diffuse_strength")->set(0.3f);
      shader_prog_->uniform_variable("specular_strength")->set(0.4f);

      object_t::draw_buffer();
   }

private:
   camera_ptr_t camera_;
};

int main( int argc, char ** argv ) {
   examples_gui_t gui(argc, argv, "Custom Object Example");

   frame_ptr_t frame = std::make_shared<frame_t>();
   frame->set_background_color(glm::vec3(0, 0, 0));
   light_engine->add_frame(frame);

   scene = std::make_shared<scene_t>();
   frame->add_scene(scene);
   user_camera = std::make_unique<user_mouse_camera_t>(scene->get_camera());
   scene->get_camera()->look_at(glm::vec3(1.5 / 2 - 0.5, 2.5 / 2 - 0.5, 0));
   scene->get_camera()->pos(glm::vec3(1.5 / 2 - 0.5, 2.5 / 2 - 0.5, 2));

   object_ptr_t obj = std::make_shared<le_logo_t>(scene->get_camera());
   scene->add_object(obj);

   gui.start();

   return 0;
}