#include <sstream>

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>

#include <glm/vec3.hpp>
#include <light_engine.hpp>
#include <frame/scene/object/builtin_objects.hpp>
#include <utils/user_camera.hpp>

namespace py = pybind11;

PYBIND11_MODULE(pyle, m) {
   m.doc() = "The python bindings for Light Engine";

   py::class_<glm::vec3>(m, "Vec3")
      .def(py::init<float, float, float>())
      .def(py::self + py::self)
      .def(py::self - py::self)
      .def(py::self += py::self)
      .def(py::self -= py::self)
      .def(float() * py::self)
      .def(py::self * float())
      .def(py::self / float())
      .def(py::self * py::self)
      .def(-py::self)
      .def("__str__", [](glm::vec3 const * v) { std::ostringstream res; res << "[" << v->x << ", " << v->y << ", " << v->z << "]"; return res.str(); })
      .def_readwrite("x", &glm::vec3::x)
      .def_readwrite("y", &glm::vec3::y)
      .def_readwrite("z", &glm::vec3::z);
   
   py::class_<LE::light_engine_t>(m, "Engine")
      .def(py::init<>())
      .def("resize", &LE::light_engine_t::resize, py::arg("height"), py::arg("width"), "Resize render window")
      .def("redraw", &LE::light_engine_t::redraw, "Draw frame")
      .def("add_frame", &LE::light_engine_t::add_frame, py::arg("frame"), "Add frame to render window")
      .def_readonly_static("__version__", &LE::light_engine_t::version, "Version of the build");

   py::class_<LE::frame_t, LE::frame_ptr_t>(m, "Frame")
      .def(py::init<>())
      .def("set_background_color", &LE::frame_t::set_background_color, py::arg("background color"), "Set background color")
      .def("draw", &LE::frame_t::set_background_color, "Draw frame")
      .def("resize", &LE::frame_t::resize, py::arg("width"), py::arg("height"), "Resize frame")
      .def("enable_depth_test", &LE::frame_t::enable_depth_test, py::arg("is_enable"), "Enable depth test for frame. Default: true")
      .def("add_scene", &LE::frame_t::add_scene, py::arg("scene"), "Add scene to frame");

   py::class_<LE::scene_t, LE::scene_ptr_t>(m, "Scene")
      .def(py::init<>())
      .def("add_object", &LE::scene_t::add_object, py::arg("object"), "Add object to scene")
      .def("draw", &LE::scene_t::draw, "Draw scene")
      .def("set_camera", &LE::scene_t::set_camera, py::arg("camera"), "Set camera for scene")
      .def("get_camera", &LE::scene_t::get_camera, "Get scene camera");

   py::class_<LE::camera_t, LE::camera_ptr_t>(m, "Camera")
      .def(py::init<>())
      .def(py::init<glm::vec3 const &>(), py::arg("position"))
      .def(py::init<glm::vec3 const &, glm::vec3 const &>(), py::arg("position"), py::arg("dir_vec"))
      .def(py::init<glm::vec3 const &, glm::vec3 const &, glm::vec3 const &>(), py::arg("position"), py::arg("dir_vec"), py::arg("up_vec"))
      .def("rotate", &LE::camera_t::rotate, py::arg("angle"), py::arg("axis"), "Rotate camera around axix for angle")
      .def("move", &LE::camera_t::move, py::arg("offset"), "Move camera by offset. LookAt vector also will moved.")
      .def("dir", (void (LE::camera_t:: *)(glm::vec3 const &)) &LE::camera_t::dir, py::arg("dir_vec"), "Set direction vector")
      .def("dir", (glm::vec3(LE::camera_t:: *)() const) &LE::camera_t::dir, "Get direction vector")
      .def("up", (void (LE::camera_t:: *)(glm::vec3 const &)) &LE::camera_t::up, py::arg("up_vec"), "Set up vector")
      .def("up", (glm::vec3(LE::camera_t:: *)() const) &LE::camera_t::up, "Get up vector")
      .def("right", (void (LE::camera_t:: *)(glm::vec3 const &)) &LE::camera_t::right, py::arg("right_vec"), "Set right vector")
      .def("right", (glm::vec3(LE::camera_t:: *)() const) &LE::camera_t::right, "Get right vector")
      .def("look_at", (void (LE::camera_t:: *)(glm::vec3 const &)) &LE::camera_t::look_at, py::arg("look_at_point"), "Set look_at point")
      .def("look_at", (glm::vec3(LE::camera_t:: *)() const) &LE::camera_t::look_at, "Get look_at point")
      .def("pos", (void (LE::camera_t:: *)(glm::vec3 const &)) &LE::camera_t::pos, py::arg("position_point"), "Set position point")
      .def("pos", (glm::vec3(LE::camera_t:: *)() const) &LE::camera_t::pos, "Get position point")
      .def("height", (void (LE::camera_t:: *)(unsigned int)) &LE::camera_t::height, py::arg("height"), "Set height")
      .def("height", (unsigned int (LE::camera_t:: *)() const) &LE::camera_t::height, "Get height")
      .def("width", (void (LE::camera_t:: *)(unsigned int)) &LE::camera_t::width, py::arg("width"), "Set width")
      .def("width", (unsigned int (LE::camera_t:: *)() const) &LE::camera_t::width, "Get width");

   py::class_<LE::object_t, LE::object_ptr_t>(m, "Object")
      .def(py::init<LE::buffer_ptr_t const &>())
      .def("draw", &LE::object_t::draw, "Draw object");

   py::class_<LE::builtin_objects_t>(m, "BuiltinObjects")
      .def_static("point", &LE::builtin_objects_t::point, py::arg("position"), py::arg("color"))
      .def_static("triangle", &LE::builtin_objects_t::triangle, py::arg("vertices"), py::arg("color"))
      .def_static("sphere", &LE::builtin_objects_t::sphere, py::arg("center"), py::arg("radius"), py::arg("color"), py::arg("detalisation"))
      .def_static("quad", &LE::builtin_objects_t::quad, py::arg("vertices"), py::arg("color"))
      .def_static("box", &LE::builtin_objects_t::triangle, py::arg("center"), py::arg("right"), py::arg("up"), py::arg("forward"), py::arg("color"))
      .def_static("point_cloud", &LE::builtin_objects_t::point_cloud, py::arg("positions"), py::arg("color"))
      .def_static("line", &LE::builtin_objects_t::triangle, py::arg("v0"), py::arg("v1"), py::arg("color"));

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
      .def("process_wheel", &LE::Utils::user_mouse_camera_t::process_wheel, py::arg("scrolls_count"), "Process mouse wheel scrolls")
      .def("process_mouse_press", &LE::Utils::user_mouse_camera_t::process_mouse_press, py::arg("x_pos"), py::arg("y_pos"), py::arg("is_left_button"), py::arg("is_pressed"), "Detect mouse press");
}
