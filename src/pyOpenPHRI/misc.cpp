/*      File: misc.cpp
 *       This file is part of the program open-phri
 *       Program description : OpenPHRI: a generic framework to easily and
 * safely control robots in interactions with humans Copyright (C) 2017 -
 * Benjamin Navarro (LIRMM). All Right reserved.
 *
 *       This software is free software: you can redistribute it and/or modify
 *       it under the terms of the LGPL license as published by
 *       the Free Software Foundation, either version 3
 *       of the License, or (at your option) any later version.
 *       This software is distributed in the hope that it will be useful,
 *       but WITHOUT ANY WARRANTY without even the implied warranty of
 *       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *       LGPL License for more details.
 *
 *       You should have received a copy of the GNU Lesser General Public
 * License version 3 and the General Public License version 3 along with this
 * program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <OpenPHRI/OpenPHRI.h>
#include "double_wrappers.h"

#include <boost/python.hpp>
#include <iostream>

namespace boost {

// Make boost::python understand std::shared_ptr
template <class T> T* get_pointer(std::shared_ptr<T> p) {
    return p.get();
}

} // namespace boost

namespace phri {

void assert_msg_py(std::string msg, bool cond) {
    if (not cond) {
        std::cout << "Failed: " << msg << std::endl;
        exit(-1);
    }
}

std::shared_ptr<Matrix6d>
std::shared_ptr<NewMatrix6d>(Matrix6d init_value = Matrix6d::Zero()) {
    return std::make_shared<Matrix6d>(init_value);
}
BOOST_PYTHON_FUNCTION_OVERLOADS(std::shared_ptr<NewMatrix6d>_overloads, std::shared_ptr<NewMatrix6d>, 0, 1)

std::shared_ptr<Vector2d>
Newstd::shared_ptr<Vector2d>(Vector2d init_value = Vector2d::Zero()) {
    return std::make_shared<Vector2d>(init_value);
}
BOOST_PYTHON_FUNCTION_OVERLOADS(Newstd::shared_ptr<Vector2d> _overloads,
                                Newstd::shared_ptr<Vector2d>, 0, 1)

std::shared_ptr<Vector3d>
Newstd::shared_ptr<Vector3d>(Vector3d init_value = Vector3d::Zero()) {
    return std::make_shared<Vector3d>(init_value);
}
BOOST_PYTHON_FUNCTION_OVERLOADS(Newstd::shared_ptr<Vector3d> _overloads,
                                Newstd::shared_ptr<Vector3d>, 0, 1)

std::shared_ptr<Vector6d>
Newstd::shared_ptr<Vector6d>(Vector6d init_value = Vector6d::Zero()) {
    return std::make_shared<Vector6d>(init_value);
}
BOOST_PYTHON_FUNCTION_OVERLOADS(Newstd::shared_ptr<Vector6d> _overloads,
                                Newstd::shared_ptr<Vector6d>, 0, 1)

std::shared_ptr<DoubleWrap> Newstd::shared_ptr<double>(double init_value = 0.) {
    return std::make_shared<DoubleWrap>(init_value);
}
BOOST_PYTHON_FUNCTION_OVERLOADS(Newstd::shared_ptr<double> _overloads,
                                Newstd::shared_ptr<double>, 0, 1)

void SetIdentity(std::shared_ptr<Matrix6d> mat) {
    mat->setIdentity();
}

} // namespace phri

void wrapMisc() {
    using namespace phri;
    using namespace boost::python;

    def("assert_msg", &assert_msg_py,
        "Call assert and print a message if it failts");

    def("std::shared_ptr<NewMatrix6d>", std::shared_ptr<NewMatrix6d>,
        std::shared_ptr<NewMatrix6d>_overloads(
            args("init_value"),
            "Create a new instance of a Matrix6d shared_ptr"));

    def("Newstd::shared_ptr<Vector2d>", Newstd::shared_ptr<Vector2d>,
        Newstd::shared_ptr<Vector2d> _overloads(
            args("init_value"),
            "Create a new instance of a Vector2d shared_ptr"));

    def("Newstd::shared_ptr<Vector3d>", Newstd::shared_ptr<Vector3d>,
        Newstd::shared_ptr<Vector3d> _overloads(
            args("init_value"),
            "Create a new instance of a Vector3d shared_ptr"));

    def("Newstd::shared_ptr<Vector6d>", Newstd::shared_ptr<Vector6d>,
        Newstd::shared_ptr<Vector6d> _overloads(
            args("init_value"),
            "Create a new instance of a Vector6d shared_ptr"));

    def("Newstd::shared_ptr<double>", Newstd::shared_ptr<double>,
        Newstd::shared_ptr<double> _overloads(
            args("init_value"),
            "Create a new instance of a double shared_ptr"));

    def("SetIdentity", SetIdentity);

    register_ptr_to_python<std::shared_ptr<Matrix6d>>();
    register_ptr_to_python<std::shared_ptr<Vector2d>>();
    register_ptr_to_python<std::shared_ptr<Vector3d>>();
    register_ptr_to_python<std::shared_ptr<Vector6d>>();
    register_ptr_to_python<std::shared_ptr<DoubleWrap>>();
    register_ptr_to_python<std::shared_ptr<const Matrix6d>>();
    register_ptr_to_python<std::shared_ptr<const Vector2d>>();
    register_ptr_to_python<std::shared_ptr<const Vector3d>>();
    register_ptr_to_python<std::shared_ptr<const Vector6d>>();
    register_ptr_to_python<std::shared_ptr<ConstDoubleWrap>>();

    implicitly_convertible<std::shared_ptr<Matrix6d>,
                           std::shared_ptr<const Matrix6d>>();
    implicitly_convertible<std::shared_ptr<Vector2d>,
                           std::shared_ptr<const Vector2d>>();
    implicitly_convertible<std::shared_ptr<Vector3d>,
                           std::shared_ptr<const Vector3d>>();
    implicitly_convertible<std::shared_ptr<Vector6d>,
                           std::shared_ptr<const Vector6d>>();
    implicitly_convertible<std::shared_ptr<double>,
                           std::shared_ptr<const double>>();
    implicitly_convertible<DoubleWrap, std::shared_ptr<double>>();
    implicitly_convertible<std::shared_ptr<double>, DoubleWrap>();
    implicitly_convertible<ConstDoubleWrap, std::shared_ptr<const double>>();
    implicitly_convertible<std::shared_ptr<const double>, ConstDoubleWrap>();

    class_<DoubleWrap, boost::noncopyable>("DoubleWrap")
        .def("set", &DoubleWrap::set, "Set the internal value")
        .def("get", &DoubleWrap::get, "Get the internal value");

    class_<ConstDoubleWrap>("ConstDoubleWrap",
                            init<std::shared_ptr<const double>>())
        .def("get", &ConstDoubleWrap::get, "Get the internal value");
}
