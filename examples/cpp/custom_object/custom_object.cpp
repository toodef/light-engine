#include <light_engine.hpp>
#include <gui_common.hpp>

using namespace LE;

class le_logo_t : public object_t {
public:
   le_logo_t(camera_ptr_t const & camera) : object_t(create_buffer(), create_shader_prog()), camera_(camera) {
      set_drawing_style(object_t::DS_triangles);
   }

   inline static buffer_ptr_t create_buffer() {
      static const glm::vec3 color(0, 1, 0);

      glm::vec3 l_offset(-0.5, -0.5, 0);
      std::vector<glm::vec3> vertices = {
         // E word
         glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), glm::vec3(1, 0.4, 0), glm::vec3(0.4, 0.4, 0),
         glm::vec3(0.4, 1 - 0.2, 0), glm::vec3(0.7, 1 - 0.2, 0), glm::vec3(0.7, 1 + 0.2, 0), glm::vec3(0.4, 1 + 0.2, 0),
         glm::vec3(0.4, 1.6, 0), glm::vec3(1, 1.6, 0), glm::vec3(1, 2, 0), glm::vec3(0, 2, 0),

         // L word
         glm::vec3(0, 0, 0) + l_offset, glm::vec3(1, 0, 0) + l_offset, glm::vec3(1, 0.4, 0) + l_offset,
         glm::vec3(0.4, 0.4, 0) + l_offset, glm::vec3(0.4, 2, 0) + l_offset,
         glm::vec3(0, 2, 0) + l_offset
      };

      std::vector<unsigned int> indices {
         // E word
         0, 1, 3,
         1, 2, 3,
         0, 3, 8,
         4, 5, 6,
         4, 6, 7,
         8, 9, 10,
         8, 10, 11,
         0, 8, 11,
         // L word
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

private:
   static shader_prog_ptr_t create_shader_prog() {
      const std::string v_shader_src = "#version 420\n"
         "layout ( location = 0 )in vec3 pos;\n"
         "layout ( location = 3 )in vec3 col;\n"
         "layout ( location = 6 )in vec3 normal;\n"

         "out vec3 vert_pos;\n"
         "out vec3 vert_col;\n"
         "out vec3 vert_normal;\n"
         "out vec3 light_pos;\n"

         "uniform mat4 mvp;\n"
         "uniform mat4 normal_matrix;\n"
         "uniform mat4 model_view;\n"
         "uniform vec3 light_position;\n"

         "void main()\n"
         "{\n"
         "   vert_col = col;\n"
         "   vert_pos = vec3(model_view * vec4(pos, 1));\n"
         "   vert_normal = mat3(normal_matrix) * normal;\n"
         "   light_pos = vec3(model_view * vec4(light_position, 1));\n"
         "   gl_Position = mvp * vec4(pos, 1.0f);\n"
         "}";

      const std::string f_shader_src = "#version 420\n"
         "in vec3 vert_pos;\n"
         "in vec3 vert_col;\n"
         "in vec3 vert_normal;\n"
         "in vec3 light_pos;\n"

         "layout ( location = 0 ) out vec4 frag_color;\n"

         "uniform vec3 light_color;\n"
         "uniform float ambient_strength;\n"
         "uniform float diffuse_strength;\n"
         "uniform float specular_strength;\n"

         "void main()\n"
         "{\n"
         // ambient
         "   vec3 ambient = ambient_strength * light_color;\n"

         // diffuse
         "   vec3 norm = normalize(vert_normal);\n"
         "   vec3 lightDir = normalize(light_pos - vert_pos);\n"
         "   float diff = max(dot(norm, lightDir), 0.0);\n"
         "   vec3 diffuse = diffuse_strength * diff * light_color;\n"

         // specular
         "   vec3 viewDir = normalize(-vert_pos);\n"
         "   vec3 reflectDir = reflect(-lightDir, norm);\n"
         "   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);\n"
         "   vec3 specular = specular_strength * spec * light_color;\n"

         "   vec3 result = (ambient + diffuse + specular) * vert_col; \n"
         "   frag_color = vec4(result, 1.0); \n"
         "}";

      shader_ptr_t vertex_shader = std::make_shared<shader_t>(v_shader_src, shader_t::ST_vertex),
                   fragment_shader = std::make_shared<shader_t>(f_shader_src, shader_t::ST_fragment);
      return std::make_shared<shader_prog_t>(vertex_shader, fragment_shader);
   }
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