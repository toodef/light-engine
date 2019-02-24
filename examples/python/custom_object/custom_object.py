from random import uniform

import pyle
from PySide2Wrapper.PySide2Wrapper.window import MainWindow
from PySide2Wrapper.PySide2Wrapper.widget import OpenGLWidget
from PySide2Wrapper.PySide2Wrapper.app import Application


class LELogo(pyle.Object):
    def __init__(self, camera):
        l_offset = pyle.Vec3(-0.5, -0.5, 0)
        vertices = [
            # E word
            pyle.Vec3(0, 0, 0), pyle.Vec3(1, 0, 0), pyle.Vec3(1, 0.4, 0), pyle.Vec3(0.4, 0.4, 0),
            pyle.Vec3(0.4, 1 - 0.2, 0), pyle.Vec3(0.7, 1 - 0.2, 0), pyle.Vec3(0.7, 1 + 0.2, 0), pyle.Vec3(0.4, 1 + 0.2, 0),
            pyle.Vec3(0.4, 1.6, 0), pyle.Vec3(1, 1.6, 0), pyle.Vec3(1, 2, 0), pyle.Vec3(0, 2, 0),

            # L word
            pyle.Vec3(0, 0, 0) + l_offset, pyle.Vec3(1, 0, 0) + l_offset, pyle.Vec3(1, 0.4, 0) + l_offset,
            pyle.Vec3(0.4, 0.4, 0) + l_offset, pyle.Vec3(0.4, 2, 0) + l_offset,
            pyle.Vec3(0, 2, 0) + l_offset
            ]

        normales = [pyle.Vec3(0, 0, 1) for i in range(len(vertices))]

        indices = [
            # E word
            0, 1, 3,
            1, 2, 3,
            0, 3, 8,
            4, 5, 6,
            4, 6, 7,
            8, 9, 10,
            8, 10, 11,
            0, 8, 11,
            # L word
            12 + 0, 12 + 1, 12 + 3,
            12 + 1, 12 + 2, 12 + 3,
            12 + 0, 12 + 3, 12 + 4,
            12 + 0, 12 + 4, 12 + 5
        ]

        buffer = pyle.Buffer(vertices, normales, pyle.Vec3(0, 1, 0))
        buffer.add_index_buffer(indices)

        shader_prog = pyle.ShaderProg(pyle.Shader(self.get_vertex_shader_src(), pyle.Shader.ShaderType.VERTEX),
                                           pyle.Shader(self.get_fragment_shader_src(), pyle.Shader.ShaderType.FRAGMENT))

        super().__init__(buffer, shader_prog)
        self.set_drawing_style(pyle.Object.DrawingStyle.DS_triangles)

        self.camera = camera
        self.set_uniforms_callback(self.uniform_callback)

    def uniform_callback(self, shader_prog):
        shader_prog.uniform_variable("mvp").set(self.camera.model_view_projection_matrix())
        shader_prog.uniform_variable("normal_matrix").set(self.camera.normal_matrix())
        shader_prog.uniform_variable("model_view").set(self.camera.model_view_matrix())
        shader_prog.uniform_variable("light_position").set(pyle.Vec3(0, 1, 10))
        shader_prog.uniform_variable("light_color").set(pyle.Vec3(1, 1, 1))
        shader_prog.uniform_variable("ambient_strength").set(0.2)
        shader_prog.uniform_variable("diffuse_strength").set(0.3)
        shader_prog.uniform_variable("specular_strength").set(0.4)

    @staticmethod
    def get_vertex_shader_src():
        return "#version 420\n" \
               "layout ( location = 0 )in vec3 pos;\n" \
               "layout ( location = 3 )in vec3 col;\n" \
               "layout ( location = 6 )in vec3 normal;\n" \
 \
               "out vec3 vert_pos;\n" \
               "out vec3 vert_col;\n" \
               "out vec3 vert_normal;\n" \
               "out vec3 light_pos;\n" \
 \
               "uniform mat4 mvp;\n" \
               "uniform mat4 normal_matrix;\n" \
               "uniform mat4 model_view;\n" \
               "uniform vec3 light_position;\n" \
 \
               "void main()\n" \
               "{\n" \
               "   vert_col = col;\n" \
               "   vert_pos = vec3(model_view * vec4(pos, 1));\n" \
               "   vert_normal = mat3(normal_matrix) * normal;\n" \
               "   light_pos = vec3(model_view * vec4(light_position, 1));\n" \
               "   gl_Position = mvp * vec4(pos, 1.0f);\n" \
               "}"

    @staticmethod
    def get_fragment_shader_src():
        return "#version 420\n" \
               "in vec3 vert_pos;\n" \
               "in vec3 vert_col;\n" \
               "in vec3 vert_normal;\n" \
               "in vec3 light_pos;\n" \
 \
               "layout ( location = 0 ) out vec4 frag_color;\n" \
 \
               "uniform vec3 light_color;\n" \
               "uniform float ambient_strength;\n" \
               "uniform float diffuse_strength;\n" \
               "uniform float specular_strength;\n" \
 \
               "void main()\n" \
               "{\n" \
 \
               "   vec3 ambient = ambient_strength * light_color;\n" \
 \
               "   vec3 norm = normalize(vert_normal);\n" \
               "   vec3 lightDir = normalize(light_pos - vert_pos);\n" \
               "   float diff = max(dot(norm, lightDir), 0.0);\n" \
               "   vec3 diffuse = diffuse_strength * diff * light_color;\n" \
 \
               "   vec3 viewDir = normalize(-vert_pos);\n" \
               "   vec3 reflectDir = reflect(-lightDir, norm);\n" \
               "   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);\n" \
               "   vec3 specular = specular_strength * spec * light_color;\n" \
 \
               "   vec3 result = (ambient + diffuse + specular) * vert_col; \n" \
               "   frag_color = vec4(result, 1.0); \n" \
               "}"


class SimpleScene:
    def __init__(self):
        self.engine = None
        self.user_camera = None

    def init(self):
        self.engine = pyle.Engine()
        frame = pyle.Frame()
        scene = pyle.Scene()
        frame.add_scene(scene)
        self.engine.add_frame(frame)
        self.user_camera = pyle.UserMouseCamera(scene.get_camera())
        frame.set_background_color(pyle.Vec3(0, 0, 0))
        scene.get_camera().look_at(pyle.Vec3(1.5 / 2 - 0.5, 2.5 / 2 - 0.5, 0))
        scene.get_camera().pos(pyle.Vec3(1.5 / 2 - 0.5, 2.5 / 2 - 0.5, 2))

        obj = LELogo(scene.get_camera())
        scene.add_object(obj)

    def resize(self, w, h):
        self.engine.resize(w, h)

    def draw(self):
        self.engine.redraw()

    def process_mouse_press(self, x, y, is_left, is_pressed):
        self.user_camera.process_mouse_press(x, y, is_left, is_pressed)

    def process_cursore_movement(self, x, y):
        self.user_camera.process_cursore_movement(x, y)

    def process_wheel(self, scrolls_count):
        self.user_camera.process_wheel(scrolls_count)


if __name__ == "__main__":
    simple_scene = SimpleScene()

    app = Application()

    gl_widget = OpenGLWidget(simple_scene.init, simple_scene.resize, simple_scene.draw)
    gl_widget.set_mouse_press_callback(simple_scene.process_mouse_press)
    gl_widget.set_mouse_move_callback(simple_scene.process_cursore_movement)
    gl_widget.set_wheel_scroll_event(simple_scene.process_wheel)

    main_window = MainWindow("Custom Object Example")
    main_window.add_widget(gl_widget, need_stretch=False)
    main_window.resize(800, 700)
    main_window.move(100, 100)
    main_window.show()

    app.run()
