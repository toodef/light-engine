#include <light_engine.hpp>
#include <gui_common.hpp>

using namespace LE;

class lighted_cube_t : public object_t {
public:
   lighted_cube_t(camera_ptr_t const & camera) : camera_(camera) {
      static const glm::vec3 center(0, 0, 0), color(0, 1, 0);
      static const float radius = 0.5;
      std::vector<glm::vec3> vertices(12);
      std::vector<unsigned int> index_data;
      std::vector<unsigned int> end_index_data{
         0 , 4 , 1 ,
         0 , 9 , 4 ,
         9 , 5 , 4 ,
         4 , 5 , 8 ,
         4 , 8 , 1 ,
         8 , 10, 1 ,
         8 , 3 , 10,
         5 , 3 , 8 ,
         5 , 2 , 3 ,
         2 , 7 , 3 ,
         7 , 10, 3 ,
         7 , 6 , 10,
         7 , 11, 6 ,
         11, 0 , 6 ,
         0 , 1 , 6 ,
         6 , 1 , 10,
         9 , 0 , 11,
         9 , 11, 2 ,
         9 , 2 , 5 ,
         7 , 2 , 11 };

      float x = 0.525731112119133606f;
      float z = 0.850650808352039932f;

      vertices[0] = glm::normalize(glm::vec3(-x, 0, z));
      vertices[1] = glm::normalize(glm::vec3(x, 0, z));
      vertices[2] = glm::normalize(glm::vec3(-x, 0, -z));
      vertices[3] = glm::normalize(glm::vec3(x, 0, -z));
      vertices[4] = glm::normalize(glm::vec3(0, z, x));
      vertices[5] = glm::normalize(glm::vec3(0, z, -x));
      vertices[6] = glm::normalize(glm::vec3(0, -z, x));
      vertices[7] = glm::normalize(glm::vec3(0, -z, -x));
      vertices[8] = glm::normalize(glm::vec3(z, x, 0));
      vertices[9] = glm::normalize(glm::vec3(-z, x, 0));
      vertices[10] = glm::normalize(glm::vec3(z, -x, 0));
      vertices[11] = glm::normalize(glm::vec3(-z, -x, 0));

      const unsigned int detail_level = 8;

      for (size_t n = 0; n < detail_level - 1; ++n) {
         for (size_t i = 0; i < end_index_data.size() - 2; i += 3) {
            size_t i0 = end_index_data[i];
            size_t i1 = end_index_data[i + 1];
            size_t i2 = end_index_data[i + 2];

            glm::vec3 v_m01 = glm::normalize(vertices[i0] + vertices[i1]);
            glm::vec3 v_m12 = glm::normalize(vertices[i1] + vertices[i2]);
            glm::vec3 v_m02 = glm::normalize(vertices[i0] + vertices[i2]);

            size_t m01 = vertices.size();
            vertices.push_back(v_m01);

            size_t m12 = vertices.size();
            vertices.push_back(v_m12);

            size_t m02 = vertices.size();
            vertices.push_back(v_m02);

            index_data.push_back(i0);
            index_data.push_back(m01);
            index_data.push_back(m02);
            index_data.push_back(i1);
            index_data.push_back(m12);
            index_data.push_back(m01);
            index_data.push_back(i2);
            index_data.push_back(m02);
            index_data.push_back(m12);
            index_data.push_back(m02);
            index_data.push_back(m01);
            index_data.push_back(m12);
         }

         end_index_data.clear();
         end_index_data = index_data;
         index_data.clear();
      }

      std::vector<glm::vec3> normales(vertices.size());
      for (unsigned int i = 0; i < normales.size(); ++i) {
         vertices[i] = vertices[i] * radius + center;
         normales[i] = glm::normalize(vertices[i] - center);
      }

      buffer_ptr_t buffer = std::make_shared<buffer_t>(vertices, normales, color);
      buffer->add_index_buffer(end_index_data);
      buffer_ = buffer;
      shader_prog_ = lighted_cube_t::create_shader_prog();
      set_drawing_style(object_t::DS_triangles);
   }

   void draw_buffer() const {
      shader_prog_->uniform_variable("mvp")->set(camera_->model_view_projection_matrix());
      shader_prog_->uniform_variable("normal_matrix")->set(camera_->normal_matrix());
      shader_prog_->uniform_variable("model_view")->set(camera_->model_view_matrix());
      shader_prog_->uniform_variable("light_position")->set(glm::vec3(3, 3, 3));
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

//private:
//   static shader_prog_ptr_t light_shader_prog_ = lighted_cube_t::create_shader_prog();
};

int main( int argc, char ** argv ) {
   examples_gui_t gui(argc, argv, "Custom Object");

   frame_ptr_t frame = std::make_shared<frame_t>();
   frame->set_background_color(glm::vec3(0, 0, 0));
   light_engine->add_frame(frame);

   scene = std::make_shared<scene_t>();
   frame->add_scene(scene);
   user_camera = std::make_unique<user_mouse_camera_t>(scene->get_camera());
   scene->get_camera()->look_at(glm::vec3(0, 0, 0));

   object_ptr_t obj = std::make_shared<lighted_cube_t>(scene->get_camera());
   scene->add_object(obj);

   gui.start();

   return 0;
}