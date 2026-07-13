#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <memory>
#include <pybind11/operators.h>
#include <pybind11/numpy.h>

#include "Vector3.h"
#include "GameObject.h"
#include "World.h"
#include "Component.h"
#include "Transform.h"
#include "Cache.h"
#include "Rigidbody.h"


namespace py = pybind11;

class PyComponent : public Component {
public:
    using Component::Component;

    void Update(double dt) override {
        PYBIND11_OVERRIDE(
            void,       // Return type
            Component,  // Parent class
            Update,     // Function name
            dt          // Arguments
        );
    }

    void Start() override {
        PYBIND11_OVERRIDE(
            void,
            Component,
            Start
        );
    }
};

class PyWorld : public World {
public:
    using World::World;

    void PythonUpdate(const std::list<GameObject *>& game_objects) const override {
        PYBIND11_OVERRIDE(
            void,
            World,
            PythonUpdate,
            game_objects
        );
    }
};

PYBIND11_MODULE(bereshitCore, m) {
    py::class_<Vector3>(m, "Vector3")
        .def(py::init<float, float, float>())
        .def(py::init<>())
        .def("magnitude", &Vector3::magnitude)
        .def("to_np", [](const Vector3& v) {
            auto a = v.ToArray();
            return py::array_t<double>(a.size(), a.data());
        })
        .def("__copy__", [](const Vector3& self) {
        return Vector3(self.x, self.y, self.z);
    })
        .def("__mul__",
        py::overload_cast<double>(&Vector3::operator*, py::const_))

        .def("__mul__",
            py::overload_cast<const Vector3&>(&Vector3::operator*, py::const_))
        .def_readwrite("x", &Vector3::x)
        .def_readwrite("y", &Vector3::y)
        .def_readwrite("z", &Vector3::z);
    py::class_<Quaternion>(m, "Quaternion")
        .def_readwrite("x", &Quaternion::x)
        .def_readwrite("y", &Quaternion::y)
        .def_readwrite("z", &Quaternion::z)
        .def("rotate", &Quaternion::Rotate)
        .def("to_matrix3", &Quaternion::ToMatrix3)
        .def_readwrite("w", &Quaternion::w);

    py::class_<Transform>(m, "Transform")
        .def_readwrite("position", &Transform::position)
        .def_readwrite("rotation", &Transform::rotation)
        .def_readwrite("size", &Transform::scale)
        .def_readwrite("quaternion", &Transform::quaternion);


    py::class_<GameObject>(m, "GameObject")
    .def(py::init<
        const Vector3&,
        const Vector3&,
        const Vector3&,
        const std::list<GameObject*>
    >(),
        py::arg("position") = Vector3(),
        py::arg("rotation") = Vector3(),
        py::arg("scale") = Vector3(),
        py::arg("children") = std::list<GameObject*>()
    )
    .def("search_by_component", &GameObject::search_by_component)
    .def("search_by_component", &GameObject::search_by_component)
    .def_property_readonly("get_all_children", &GameObject::GetComponents)
    .def("get_component",static_cast<std::shared_ptr<Component> (GameObject::*)(const std::string&)>(&GameObject::GetComponent))
    .def("add_component", &GameObject::AddComponent,
         py::return_value_policy::reference)
    .def_property("World", &GameObject::GetWorld, &GameObject::setWorld)
    .def_readwrite("name", &GameObject::name)
    .def_readwrite("Cache", &GameObject::cache)
    .def_readwrite("transform", &GameObject::transform)
    .def("__getattr__", [](GameObject& self, const std::string& name) {
        auto comp = self.GetComponent(name); // returns std::shared_ptr<Component>

        if (!comp) {
            throw py::attribute_error(
                ("GameObject has no component named '" + name + "'").c_str()
            );
        }

        return comp;
    })
    .def_readwrite("components", &GameObject::components);

    py::class_<World, PyWorld>(m, "World")
    .def_readwrite("tick", &World::tick)
    .def_readwrite("speed", &World::speed)
    .def("search_by_component", &World::search_by_component)
    .def(py::init<bool*,std::list<GameObject*>,GameObject*,Vector3,double,double,int>())
    .def("Start", &World::Start)
    .def("Exit", &World::Exit)
    .def("get_all_children", &World::getAllChildren)
    .def("PythonUpdate", &World::PythonUpdate)
    .def("update", &World::Update, py::arg("updateComponent") = false);

    py::class_<Component, PyComponent, std::shared_ptr<Component>>(m, "Component")
    .def(py::init<>())
    .def_property("name", &Component::GetName, &Component::SetName)
    .def("Update", &Component::Update)
    .def("Start", &Component::Start)
    .def("attach", &Component::attach);
    py::class_<Cache>(m, "Cache")
    .def("SetDirty", &Cache::SetDirty);
    py::class_<Rigidbody, Component, std::shared_ptr<Rigidbody>>(m, "Rigidbody")
    .def_readwrite("isKinematic", &Rigidbody::isKinematic)
    .def(py::init<>());



}