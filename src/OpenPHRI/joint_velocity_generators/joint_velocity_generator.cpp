/*      File: joint_velocity_generator.cpp
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

#include <OpenPHRI/joint_velocity_generators/joint_velocity_generator.h>

namespace phri {

const vector::dyn::Velocity& JointVelocityGenerator::compute() {
    velocity_.setZero();
    update(velocity_);
    return velocity_;
}

const vector::dyn::Velocity& JointVelocityGenerator::operator()() {
    return compute();
}

void JointVelocityGenerator::setRobot(Robot const* robot) {
    robot_ = robot;
    velocity_.resize(robot_->jointCount());
}

const Robot& JointVelocityGenerator::robot() const {
    return *robot_;
}

} // namespace phri