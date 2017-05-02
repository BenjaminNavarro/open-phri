/*
 *  Copyright (C) 2017 Benjamin Navarro <contact@bnavarro.info>
 *
 *  This file is part of RSCL <https://gite.lirmm.fr/navarro/RSCL>.
 *
 *  RSCL is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  RSCL is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with RSCL.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file force_constraint.h
 * @author Benjamin Navarro
 * @brief Definition of the ForceConstraint class
 * @date April 2017
 * @ingroup RSCL
 */

#pragma once

#include <RSCL/definitions.h>
#include <RSCL/constraints/constraint.h>

namespace RSCL {

/** @brief A constraint to limit the external force.
 *  @details MUST NOT BE USED FOR NOW
 */
class ForceConstraint : public Constraint {
public:
	/***		Constructor & destructor		***/

	/**
	 * @brief Construct a force constraint with a given external and maximum force.
	 * @param external_force A shared pointer to the external force.
	 * @param maximum_force A shared pointer to the maximum external force allowed.
	 */
	ForceConstraint(
		Vector6dConstPtr external_force,
		doubleConstPtr maximum_force);

	virtual ~ForceConstraint() = default;

	/***		Algorithm		***/
	virtual double compute() override;

private:
	Vector6dConstPtr external_force_;
	doubleConstPtr maximum_force_;
};

using ForceConstraintPtr = std::shared_ptr<ForceConstraint>;
using ForceConstraintConstPtr = std::shared_ptr<const ForceConstraint>;

} // namespace RSCL