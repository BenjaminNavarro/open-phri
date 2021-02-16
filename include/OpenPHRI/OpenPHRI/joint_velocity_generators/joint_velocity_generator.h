/*      File: joint_velocity_generator.h
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

//! \file joint_velocity_generator.h
//! \author Benjamin Navarro
//! \brief Base class for all joint_velocity generators
//! \date 05-2019
//! \ingroup phri

#pragma once

#include <memory>

#include <OpenPHRI/definitions.h>
#include <OpenPHRI/robot.h>
#include <OpenPHRI/fwd_decl.h>

namespace phri {

//! \brief Base class for all joint_velocity generators.
//! \details Provides a pure virtual compute method to be implemented by derived
//! classes.
class JointVelocityGenerator {
public:
    //! \brief Default constructor
    JointVelocityGenerator() = default;

    //! \brief Default copy constructor
    JointVelocityGenerator(const JointVelocityGenerator&) = default;

    //! \brief Default move constructor
    JointVelocityGenerator(JointVelocityGenerator&&) = default;

    //! \brief Default virtual destructor
    virtual ~JointVelocityGenerator() = default;

    //! \brief Default copy operator
    JointVelocityGenerator& operator=(const JointVelocityGenerator&) = default;

    //! \brief Default move operator
    JointVelocityGenerator& operator=(JointVelocityGenerator&&) = default;

    //! \brief Compute the value associated with the joint_velocity generator.
    //! \return The joint_velocity generator's evaluated value.
    virtual VectorXd compute() final;

    //! \brief Call operator, shortcut for compute().
    //! \return The joint_velocity generator's evaluated value.
    virtual VectorXd operator()() final;

protected:
    friend class SafetyController;

    //! \brief Derived classed must implement this to provide their velocity
    //! output
    //! \param velocity A reference to the velocity to set
    virtual void update(VectorXd& velocity) = 0;

    //! \brief Set the robot to work with.
    //! \param robot The robot.
    virtual void setRobot(Robot const* robot);

    //! \brief Read/write access the controlled robot
    //! \return double& A reference to the controlled robot
    virtual const Robot& robot() const final;

private:
    Robot const* robot_;

    VectorXd velocity_;
};

} // namespace phri
