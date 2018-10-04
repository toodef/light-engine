from random import uniform

import pyle
from PySide2Wrapper.PySide2Wrapper.window import MainWindow
from PySide2Wrapper.PySide2Wrapper.widget import OpenGLWidget, Button
from PySide2Wrapper.PySide2Wrapper.app import Application


class SimpleScene:
    def __init__(self):
        self.__engine = None

    def init(self):
        self.__engine = pyle.Engine()
        frame = pyle.Frame()
        scene = pyle.Scene()
        frame.add_scene(scene)
        self.__engine.add_frame(frame)
        frame.set_background_color(pyle.Vec3(0, 0, 0))

        # user_camera = pyle.UserMouseCamera(scene.get_camera())
        scene.get_camera().look_at(pyle.Vec3(0, 0, 0))

        for i in range(100):
            pos = pyle.Vec3(uniform(-0.5, 0.5), uniform(-0.5, 0.5), uniform(-0.5, 0.5))
            color = pyle.Vec3(0, 1, 0)
            obj = pyle.BuiltinObjects.point(pos, color)
            scene.add_object(obj)

    def resize(self, w, h):
        self.__engine.resize(w, h)

    def draw(self):
        self.__engine.redraw()


if __name__ == "__main__":
    simple_scene = SimpleScene()

    app = Application()
    gl_widget = OpenGLWidget(simple_scene.init, simple_scene.resize, simple_scene.draw)
    main_window = MainWindow("Simple Scene Example")
    main_window.add_widget(gl_widget, need_stretch=False)
    main_window.resize(800, 700)
    main_window.move(100, 100)
    main_window.show()
    app.run()
