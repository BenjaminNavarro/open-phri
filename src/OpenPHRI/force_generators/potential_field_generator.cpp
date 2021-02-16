/*      File: potential_field_generator.cpp
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

#include <OpenPHRI/force_generators/potential_field_generator.h>

using namespace phri;
using namespace Eigen;

PotentialFieldGenerator::PotentialFieldGenerator(ReferenceFrame objects_frame)
    : ForceGenerator(objects_frame), objects_frame_(objects_frame) {
    offset_ = std::make_shared<Vector3d>(Vector3d::Zero());
}

PotentialFieldGenerator::PotentialFieldGenerator(Vector3dConstPtr offset,
                                                 ReferenceFrame objects_frame)
    : PotentialFieldGenerator(objects_frame) {
    offset_ = offset;
}

void PotentialFieldGenerator::update(Vector6d& force) {
    Vector3d total_force = Vector3d::Zero();
    Vector3d rob_pos;

    if (objects_frame_ == ReferenceFrame::TCP) {
        rob_pos = *offset_;
    } else {
        rob_pos =
            robot_->task.state.pose.translation() +
            robot_->control.transformation_matrix.block<3, 3>(0, 0) * *offset_;
    }

    for (const auto& item : items_) {
        const PotentialFieldObject& obj = *(item.second);
        Vector3d obj_rob_vec = obj.object_position->translation() - rob_pos;

        double distance = obj_rob_vec.norm();
        if (std::abs(distance) > 1e-3) {
            Vector3d obj_rob_vec_unit = obj_rob_vec / distance;

            double gain = *obj.gain;

            if (obj.type == PotentialFieldType::Attractive) {
                total_force += gain * obj_rob_vec_unit;
            } else {
                double th = *obj.threshold_distance;
                if (distance < th) {
                    total_force +=
                        gain * (1. / th - 1. / distance) * obj_rob_vec_unit;
                }
            }
        }
    }

    force.segment<3>(0) = total_force;
    force.segment<3>(3) = Vector3d::Zero();
}
