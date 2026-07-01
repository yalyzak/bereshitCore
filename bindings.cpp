#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "Vector3.h"
#include "GameObject.h"
#include "World.h"

namespace py = pybind11;

PYBIND11_MODULE(bereshitCore, m) {
    py::class_<Vector3>(m, "Vector3")
        .def(py::init<float, float, float>())
        .def("magnitude", &Vector3::magnitude)
        .def_readwrite("x", &Vector3::x)
        .def_readwrite("y", &Vector3::y)
        .def_readwrite("z", &Vector3::z);

    py::class_<GameObject>(m, "GameObject")
    .def(py::init<>())
    .def_readwrite("name", &GameObject::name);

    py::class_<World>(m, "World")
    .def(py::init<std::list<GameObject*>>())
    .def_readwrite("children", &World::children);

}