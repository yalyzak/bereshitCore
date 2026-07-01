#include <pybind11/pybind11.h>
#include "Vector3.h"

namespace py = pybind11;

PYBIND11_MODULE(bereshitCore, m) {
    py::class_<Vector3>(m, "Vector3")
        .def(py::init<float, float, float>())
        .def("magnitude", &Vector3::magnitude)
        .def_readwrite("x", &Vector3::x)
        .def_readwrite("y", &Vector3::y)
        .def_readwrite("z", &Vector3::z);
}

