from random import uniform

import pyle
from PySide2Wrapper.PySide2Wrapper.window import MainWindow
from PySide2Wrapper.PySide2Wrapper.widget import OpenGLWidget
from PySide2Wrapper.PySide2Wrapper.app import Application


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
        scene.get_camera().look_at(pyle.Vec3(0, 0, 0))

        for i in range(1000):
            obj = pyle.BuiltinObjects.point(pyle.Vec3(uniform(-0.5, 0.5), uniform(-0.5, 0.5), uniform(-0.5, 0.5)),
                                            pyle.Vec3(uniform(0, 1), uniform(0, 1), uniform(0, 1)))
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

    main_window = MainWindow("Simple Scene Example")
    main_window.add_widget(gl_widget, need_stretch=False)
    main_window.resize(800, 700)
    main_window.move(100, 100)
    main_window.show()

    app.run()
