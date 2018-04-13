#include <pybind11/pybind11.h>

#include <glm/vec3.hpp>
#include "../include/light_engine.hpp"

namespace py = pybind11;

PYBIND11_MODULE(pyle, m) {
   py::class_<glm::vec3>(m, "Vec3")
      .def_readwrite("x", &glm::vec3::x).def_readwrite("y", &glm::vec3::y).def_readwrite("z", &glm::vec3::z);

   py::class_<LE::light_engine_t>(m, "Engine")
      .def(py::init<>())
      .def("set_background_color", &LE::light_engine_t::set_background_color, py::arg("Backgroun color"), "Set background color for frame");
}
