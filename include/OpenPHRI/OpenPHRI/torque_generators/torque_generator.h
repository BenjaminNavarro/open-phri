/*      File: torque_generator.h
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

//! \file torque_generator.h
//! \author Benjamin Navarro
//! \brief Base class for all torque generators.
//! \date 05-2019
//! \ingroup phri

#pragma once

#include <OpenPHRI/definitions.h>
#include <OpenPHRI/robot.h>
#include <OpenPHRI/fwd_decl.h>

#include <physical_quantities/vector/force.h>

namespace phri {

//! \brief Base class for all torque generators.
//! \details Provides a pure virtual compute method to be implemented by derived
//! classes.
class TorqueGenerator {
public:
    TorqueGenerator() = default;

    //! \brief Delete copy constructor to avoid slicing
    TorqueGenerator(const TorqueGenerator&) = delete;

    //! \brief Default move constructor
    TorqueGenerator(TorqueGenerator&&) = default;

    //! \brief Default virtual destructor
    virtual ~TorqueGenerator() = default;

    //! \brief Delete copy operator to avoid slicing
    TorqueGenerator& operator=(const TorqueGenerator&) = delete;

    //! \brief Default move operator
    TorqueGenerator& operator=(TorqueGenerator&&) = default;

    /**
     * @brief Compute the value associated with the torque generator.
     * @return The torque generator's evaluated value.
     */
    const vector::dyn::Force& compute();

    /**
     * @brief Call operator, shortcut for compute().
     * @return The torque generator's evaluated value.
     */
    const vector::dyn::Force& operator()();

protected:
    friend class SafetyController;

    virtual void update(vector::dyn::Force& torque) = 0;

    /**
     * @brief Set the robot to work with.
     * @param robot The robot.
     */
    virtual void setRobot(Robot const* robot);

    Robot const* robot_;

    vector::dyn::Force force_;
};

using TorqueGeneratorPtr = std::shared_ptr<TorqueGenerator>;
using TorqueGeneratorConstPtr = std::shared_ptr<const TorqueGenerator>;

} // namespace phri
