/*      File: force_control.h
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

//! \file force_control.h
//! \author Benjamin Navarro
//! \brief Generates a velocity to regulate the external force to a given target
//! using PD control \date 05-2019 \ingroup phri

#pragma once

#include <OpenPHRI/velocity_generators/velocity_generator.h>
#include <OpenPHRI/definitions.h>
#include <array>

#include <physical_quantities/scalar/cutoff_frequency.h>
#include <physical_quantities/scalar/time_constant.h>

namespace phri {

//! \brief Generates a velocity to regulate the external force to a given target
//! using PD control
class ForceControl : public VelocityGenerator {
public:
    //! \brief Specify for force target type
    enum class TargetType {
        //! \brief The target force is the one applied to the environment
        Environment,
        //! \brief The target force is the one applied to the robot
        Robot
    };

    //! \brief Contains the control parameters for the ForceControl generator
    struct Parameters {
        //! \brief Construct a new Force Control Parameters object with gains
        //! set to zero and no degrees of freedom selected
        Parameters();

        //! \brief Construct a new Force Control Parameters object with the
        //! given gains and selection vector \param proportional_gain Gain
        //! applied to the force error \param derivative_gain Gain applied to
        //! the force error derivative \param selection_vector A non-zero value
        //! indicates that equivalent degree of freedom is force controled
        Parameters(const Eigen::Vector6d& proportional_gain,
                   const Eigen::Vector6d& derivative_gain,
                   const std::array<bool, 6>& selection_vector);

        //! \brief Gain applied to the force error
        Eigen::Vector6d proportional_gain;
        //! \brief Gain applied to the force error derivative
        Eigen::Vector6d derivative_gain;
        //! \brief A non-zero value indicates that equivalent degree of freedom
        //! is force controled
        std::array<bool, 6> selection_vector;
    };

    //! \brief  Construct a new Force Control object with default parameters and
    //! with an initial target set to zero
    //! \details Use ForceControl::target() to set the desired force value
    //! \param type If the regulated force is the one applied to the environment
    //! or to the robot
    explicit ForceControl(TargetType type);

    //! \brief  Construct a new Force Control object with given pointed target
    //! and parameters
    //! \details Use ForceControl::target() to set the desired force value
    //! \param target The targeted force
    //! \param parameters The force control law parameters
    //! \param type If the regulated force is the one applied to the environment
    //! or to the robot
    ForceControl(std::shared_ptr<spatial::Force> target,
                 std::shared_ptr<Parameters> parameters, TargetType type);

    //! \brief  Construct a new Force Control object with given referenced
    //! target and parameters
    //! \details Use ForceControl::target() to set the desired force value
    //! \param target The targeted force. Make sure that \p target outlives the
    //! generator
    //! \param parameters The force control law parameters. Make sure that \p
    //! parameters outlives the generator
    //! \param type If the regulated force is the one applied to the environment
    //! or to the robot
    ForceControl(spatial::Force& target, Parameters& parameters,
                 TargetType type);

    //! \brief  Construct a new Force Control object with given target
    //! and parameters values
    //! \details Use ForceControl::target() to set the desired force value
    //! \param target The targeted force
    //! \param parameters The force control law parameters
    //! \param type If the regulated force is the one applied to the environment
    //! or to the robot
    ForceControl(const spatial::Force& target, const Parameters& parameters,

                 TargetType type);

    //! \brief  Construct a new Force Control object with given target
    //! and parameters values
    //! \details Use ForceControl::target() to set the desired force value
    //! \param target The targeted force
    //! \param parameters The force control law parameters
    //! \param type If the regulated force is the one applied to the environment
    //! or to the robot
    ForceControl(spatial::Force&& target, Parameters&& parameters,
                 TargetType type);

    //! \brief Configure the filter acting on the error
    //! \details Using this filter will provide a smoother derivative of the
    //! error at the cost of a delay. A simple first order low-pass filter is
    //! used internally.
    //! \param time_constant The time constant for the filter
    void configureFilter(scalar::TimeConstant time_constant);

    //! \brief Configure the filter acting on the error
    //! \details Using this filter will provide a smoother derivative of the
    //! error at the cost of a delay. A simple first order low-pass filter is
    //! used internally.
    //! \param cutoff_frequency The filter cutoff frequency
    void configureFilter(scalar::CutoffFrequency cutoff_frequency);

    //! \brief Read/write access the target spatial::Force used by the generator
    //! \return double& A reference to the target spatial::Force
    spatial::Force& target();

    //! \brief Read access the target spatial::Force used by the generator
    //! \return double The target spatial::Force value
    const spatial::Force& target() const;

    //! \brief Access to the shared pointer holding the target spatial::Force
    //! used by the generator \return std::shared_ptr<double> A shared pointer
    //! to the spatial::Force target
    std::shared_ptr<spatial::Force> targetPtr() const;

    //! \brief Read/write access the control parameters used by the generator
    //! \return double& A reference to the control parameters
    Parameters& parameters();

    //! \brief Read access the control parameters used by the generator
    //! \return double The target control parameters
    const Parameters& parameters() const;

    //! \brief Access to the shared pointer holding the control parameters used
    //! by the generator
    //! \return std::shared_ptr<double> A shared pointer to the control
    //! parameters
    std::shared_ptr<Parameters> parametersPtr() const;

protected:
    virtual void update(spatial::Velocity& velocity) override;
    void applySelection(Eigen::Vector6d& vec) const;

    std::shared_ptr<spatial::Force> target_;
    std::shared_ptr<Parameters> parameters_;
    TargetType type_;
    double filter_coeff_;
    spatial::Force prev_error_;
};

} // namespace phri
