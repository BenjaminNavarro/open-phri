/*      File: velocity_generator.h
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

//! \file velocity_generator.h
//! \author Benjamin Navarro
//! \brief Base class for all velocity generators.
//! \date 05-2019
//! \ingroup phri

#pragma once

#include <memory>

#include <OpenPHRI/definitions.h>
#include <OpenPHRI/robot.h>
#include <OpenPHRI/fwd_decl.h>

namespace phri {

//! \brief Base class for all velocity generators.
//! \details Provides a pure virtual compute method.
class VelocityGenerator {
public:
    //! \brief Default copy constructor
    VelocityGenerator(const VelocityGenerator&) = default;

    //! \brief Default move constructor
    VelocityGenerator(VelocityGenerator&&) = default;

    //! \brief Default virtual destructor
    virtual ~VelocityGenerator() = default;

    //! \brief Default copy operator
    VelocityGenerator& operator=(const VelocityGenerator&) = default;

    //! \brief Default move operator
    VelocityGenerator& operator=(VelocityGenerator&&) = default;

    //! \brief Compute the value associated with the velocity generator.
    //! \return The velocity generator's evaluated value.
    virtual Twist compute() final;

    //! \brief Call operator, shortcut for compute().
    //! \return The velocity generator's evaluated value.
    virtual Twist operator()() final;

protected:
    friend class SafetyController;

    //! \brief Construct a velocity generator
    //! \param frame The reference frame in which the velocity is expressed.
    VelocityGenerator(ReferenceFrame frame);

    //! \brief Transform the given velocity in the TCP frame, if necessary.
    //! \param velocity The velocity to transform.
    virtual Twist transform(const Twist& velocity) final;

    //! \brief Derived classed must implement this to provide their velocity
    //! output
    //! \param velocity A reference to the velocity to set
    virtual void update(Twist& velocity) = 0;

    //! \brief Set the robot to work with.
    //! \param robot The robot.
    virtual void setRobot(Robot const* robot);

    Robot const* robot_;
    ReferenceFrame frame_;
};

} // namespace phri
