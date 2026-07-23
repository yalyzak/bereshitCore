#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <memory>
#include <pybind11/operators.h>
#include <pybind11/numpy.h>

#include "Vector3.h"
#include "Vector2.h"
#include "GameObject.h"
#include "World.h"
#include "Component.h"
#include "Transform.h"
#include "Cache.h"
#include "Rigidbody.h"
#include "BoxCollider.h"
#include "Collider.h"
#include "Joint.h"
#include "FixedJoint.h"
#include "HingeJoint.h"


namespace py = pybind11;

class PyComponent : public Component {
public:
    using Component::Component;
    std::string GetTypeName() const override {
        py::gil_scoped_acquire gil;

        py::object self =
            py::cast(const_cast<PyComponent*>(this),
                     py::return_value_policy::reference);

        return py::str(self.attr("__class__").attr("__name__"));
    }
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


PYBIND11_MODULE(bereshitCore, m) {
    py::class_<Vector2>(m, "Vector2")
        .def(py::init<float, float>())
        .def(py::init<>());

    py::class_<Vector3>(m, "Vector3")
        .def(py::init<float, float, float>())
        .def(py::init<>())
        .def("magnitude", &Vector3::magnitude)
        .def("normalized", &Vector3::normalized)
         .def("__str__", &Vector3::toString)
        .def("__repr__", &Vector3::toString)
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
        .def("__add__",
        py::overload_cast<const Vector3&>(
            &Vector3::operator+, py::const_
        )
    )

    .def("__iadd__",
        [](Vector3& self, const Vector3& other) -> Vector3& {
            self += other;
            return self;
        },
        py::return_value_policy::reference_internal
    )
        .def_readwrite("x", &Vector3::x)
        .def_readwrite("y", &Vector3::y)
        .def_readwrite("z", &Vector3::z);

    py::class_<Quaternion>(m, "Quaternion")
        .def_readwrite("x", &Quaternion::x)
        .def_readwrite("y", &Quaternion::y)
        .def_readwrite("z", &Quaternion::z)
        .def_readwrite("w", &Quaternion::w)

    .def("__mul__",
            py::overload_cast<const Quaternion&>(&Quaternion::operator*, py::const_))

        .def("rotate", &Quaternion::Rotate)
        .def("to_matrix3",py::overload_cast<std::array<std::array<double, 3>, 3>&>(&Quaternion::ToMatrix3,py::const_))
        .def("axis_angle", &Quaternion::AxisAngle)
        .def("conjugate", &Quaternion::Conjugate)
        .def_readwrite("w", &Quaternion::w);

    py::class_<Transform>(m, "Transform")
        .def_readwrite("position", &Transform::position)
        .def_readwrite("rotation", &Transform::rotation)
        .def_readwrite("size", &Transform::scale)
        .def_readwrite("quaternion", &Transform::quaternion);


    py::class_<GameObject>(m, "GameObject")
    .def(py::init<Vector3, Vector3, Vector3, std::list<GameObject*>, std::string>(),
    py::arg("position") = Vector3(), py::arg("rotation") = Vector3(), py::arg("size") = Vector3(1, 1, 1),
     py::arg("children") = std::list<GameObject*>{}, py::arg("name") = "" )
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
    .def_property_readonly("components", &GameObject::GetComponents);

    py::class_<World>(m, "World")
    .def_readwrite("tick", &World::tick)
    .def_readwrite("speed", &World::speed)
    .def("search_by_component", &World::search_by_component)
    .def(py::init<bool*,std::list<GameObject*>,GameObject*,Vector3,double,double,int>())
    .def("Start", &World::Start)
    .def("Exit", &World::Exit)
    .def("get_all_children", &World::getAllChildren)
    .def("get_gizmos", &World::getGizmos)
    .def("update", &World::Update, py::arg("updateComponent") = false);

    py::class_<Component, PyComponent, std::shared_ptr<Component>>(m, "Component")
    .def(py::init<>())
    .def_property("name", &Component::GetName, &Component::SetName)
    .def_property_readonly("parent", &Component::GetParent)
    .def("Update", &Component::Update)
    .def("Start", &Component::Start)
    .def("attach", &Component::attach);
    py::class_<Cache>(m, "Cache")
    .def("SetDirty", &Cache::SetDirty);
    py::class_<Rigidbody, Component, std::shared_ptr<Rigidbody>>(m, "Rigidbody")
    .def_property("isKinematic", &Rigidbody::IsKinematic, &Rigidbody::SetIsKinematic)
    .def_readwrite("Freeze_Rotation", &Rigidbody::freezeRotation)
    .def_readwrite("velocity", &Rigidbody::velocity)
    .def(
    py::init<
        float,
        bool,
        Vector3,
        Vector3,
        bool,
        float,
        float,
        Vector3
    >(),
    py::arg("mass") = 1.0f,
    py::arg("isKinematic") = false,
    py::arg("velocity") = Vector3(),
    py::arg("angular_velocity") = Vector3(),
    py::arg("useGravity") = true,
    py::arg("friction_coefficient") = 0.6f,
    py::arg("restitution") = 0.6f,
    py::arg("Freeze_Rotation") = Vector3());

    py::class_<Collider, Component, std::shared_ptr<Collider>>(m, "Collider")
     .def(py::init<bool>(), py::arg("is_trigger") = false);
    py::class_<BoxCollider, Collider, std::shared_ptr<BoxCollider>>(m, "BoxCollider")
    .def(py::init<>());
    py::class_<Joint, Component, std::shared_ptr<Joint>>(m, "Joint")
    .def(
    py::init<
        GameObject*,
        Vector3*,
        double
    >(),
    py::arg("bodyB"),
    py::arg("anchor") = nullptr,
    py::arg("beta") = 0.2
);
    py::class_<FixedJoint, Joint, std::shared_ptr<FixedJoint>>(m, "FixedJoint")
    .def(
    py::init<
        GameObject*,
        Vector3*,
        double
    >(),
    py::arg("bodyB"),
    py::arg("anchor") = nullptr,
    py::arg("beta") = 0.2
);
    py::class_<HingeJoint, Joint, std::shared_ptr<HingeJoint>>(m, "HingeJoint")
    .def(py::init<GameObject*, Vector3, Vector3*, double>(),py::arg("bodyB"), py::arg("axis"),
        py::arg("anchor") = nullptr,py::arg("beta") = 0.2);





}