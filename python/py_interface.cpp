#include <pybind11/pybind11.h>

#include <glm/vec3.hpp>
#include <light_engine.hpp>
#include <frame/scene/object/builtin_objects.hpp>
#include <utils/user_camera.hpp>

namespace py = pybind11;

PYBIND11_MODULE(pyle, m) {
   py::class_<glm::vec3>(m, "Vec3")
      .def(py::init<>([](std::array<float, 3> const & vec) -> glm::vec3 { return glm::vec3({vec[0], vec[1], vec[2]}); }))
      .def_readwrite("x", &glm::vec3::x).def_readwrite("y", &glm::vec3::y).def_readwrite("z", &glm::vec3::z);

   py::class_<LE::light_engine_t>(m, "Engine")
      .def(py::init<>())
      .def("resize", &LE::light_engine_t::resize, py::arg("height"), py::arg("width"), "Resize render window")
      .def("redraw", &LE::light_engine_t::redraw, "Draw frame")
      .def("add_frame", &LE::light_engine_t::add_frame, py::arg("frame"), "Add frame to render window");

   py::class_<LE::frame_t>(m, "Frame")
      .def(py::init<>())
      .def("set_background_color", &LE::frame_t::set_background_color, py::arg("background color"), "Set background color")
      .def("draw", &LE::frame_t::set_background_color, "Draw frame")
      .def("resize", &LE::frame_t::resize, py::arg("width"), py::arg("height"), "Resize frame")
      .def("enable_depth_test", &LE::frame_t::enable_depth_test, py::arg("is_enable"), "Enable depth test for frame. Default: true");

   py::class_<LE::scene_t>(m, "Scene")
      .def(py::init<>())
      .def("add_object", &LE::scene_t::add_object, py::arg("object"), "Add object to scene")
      .def("draw", &LE::scene_t::draw, "Draw scene")
      .def("set_camera", &LE::scene_t::set_camera, py::arg("camera"), "Set camera for scene")
      .def("get_camera", &LE::scene_t::get_camera, "Get scene camera");

   py::class_<LE::object_t>(m, "Object")
      .def(py::init<LE::buffer_ptr_t const &>())
      .def("draw", &LE::object_t::draw, "Draw object");

   py::class_<LE::builtin_objects_t>(m, "BuiltinObjects")
      .def_static("point", &LE::builtin_objects_t::point, py::arg("position"), py::arg("color"));

   py::class_<LE::Utils::user_mouse_camera_t>(m, "UserMouseCamera")
      .def(py::init<LE::camera_ptr_t const &>())
      .def("process_cursore_movement", &LE::Utils::user_mouse_camera_t::process_cursore_movement, py::arg("x_pos"), py::arg("y_pos"), "Process cursore movement. Need to be placed in function, that track cursore movement. Function get current cursore position")
      .def("process_keyboard", 
         [](LE::Utils::user_mouse_camera_t & self, std::array<bool, 256> const & key_buffer) {
            bool key_buf[256];
            for (unsigned int i = 0; i < 256; ++i)
               key_buf[i] = key_buffer[i];
            self.process_keyboard(key_buf);
         }, py::arg("key_buffer"), "Process presseg keys")
      .def("process_wheel", &LE::Utils::user_mouse_camera_t::process_wheel, py::arg("scrolls_count"), "Process mouse wheel scrolls");
}
