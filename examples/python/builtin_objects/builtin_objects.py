from random import uniform

import pyle
from PySide2Wrapper.PySide2Wrapper.window import MainWindow
from PySide2Wrapper.PySide2Wrapper.widget import OpenGLWidget
from PySide2Wrapper.PySide2Wrapper.app import Application


class SimpleScene:
    cells_num = [3, 2]

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
        scene.get_camera().pos(pyle.Vec3(0, 0, -3))
        scene.get_camera().look_at(pyle.Vec3(0, 0, 0))

        min_coord = self.__calc_cur_cell_min_coord(0, 0)
        for i in range(10):
            scene.add_object(pyle.BuiltinObjects.point(pyle.Vec3(uniform(0, 1) + min_coord[0], uniform(0, 1) + min_coord[1], uniform(0, 1) - 0.5),
                                                       pyle.Vec3(0, 1, 0)))

        min_coord = self.__calc_cur_cell_min_coord(1, 0)
        for i in range(2):
            triangle_vertices = []
            for j in range(3):
                triangle_vertices.append(pyle.Vec3(uniform(0, 1) + min_coord[0], uniform(0, 1) + min_coord[1], uniform(0, 1) - 0.5))
            scene.add_object(pyle.BuiltinObjects.triangle(triangle_vertices, pyle.Vec3(1, 0, 0)))

        min_coord = self.__calc_cur_cell_min_coord(0, 1)
        scene.add_object(pyle.BuiltinObjects.sphere(pyle.Vec3(min_coord[0] + 0.5, min_coord[1] + 0.5, 0.5), 0.5, pyle.Vec3(1, 1, 1), 2))

        min_coord = self.__calc_cur_cell_min_coord(1, 1)
        quad_vertices = [
            pyle.Vec3(min_coord[0], min_coord[1], 0),
            pyle.Vec3(min_coord[0], min_coord[1] + 1, 0),
            pyle.Vec3(min_coord[0] + 1, min_coord[1] + 1, 0),
            pyle.Vec3(min_coord[0] + 1, min_coord[1], 0)
        ]
        scene.add_object(pyle.BuiltinObjects.quad(quad_vertices, pyle.Vec3(0, 0, 1)))

        min_coord = self.__calc_cur_cell_min_coord(2, 0)
        scene.add_object(pyle.BuiltinObjects.box(pyle.Vec3(min_coord[0] + 0.5, min_coord[1] + 0.5, 0),
                                                 pyle.Vec3(0, 0.5, 0), pyle.Vec3(0.5, 0, 0), pyle.Vec3(0, 0, 0.5),
                                                 pyle.Vec3(1, 0, 1)))

        min_coord = self.__calc_cur_cell_min_coord(2, 1)
        for i in range(5):
            scene.add_object(pyle.BuiltinObjects.line(pyle.Vec3(uniform(0, 1) + min_coord[0], uniform(0, 1) + min_coord[1], uniform(0, 1) - 0.5),
                                                      pyle.Vec3(uniform(0, 1) + min_coord[0], uniform(0, 1) + min_coord[1], uniform(0, 1) - 0.5),
                                                      pyle.Vec3(1, 1, 0)))

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

    def __calc_cur_cell_min_coord(self, cur_cell_x: int, cur_cell_y: int):
        return [cur_cell_x - self.cells_num[0] / 2, cur_cell_y - self.cells_num[1] / 2]


if __name__ == "__main__":
    simple_scene = SimpleScene()

    app = Application()

    gl_widget = OpenGLWidget(simple_scene.init, simple_scene.resize, simple_scene.draw)
    gl_widget.set_mouse_press_callback(simple_scene.process_mouse_press)
    gl_widget.set_mouse_move_callback(simple_scene.process_cursore_movement)
    gl_widget.set_wheel_scroll_event(simple_scene.process_wheel)

    main_window = MainWindow("Simple Scene Example")
    main_window.add_widget(gl_widget, need_stretch=False)
    main_window.resize(800, 700)
    main_window.move(100, 100)
    main_window.show()

    app.run()
