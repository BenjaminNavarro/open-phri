#include <RSCL/separation_distance_constraint.h>

#include <iostream>

using namespace RSCL;

SeparationDistanceConstraint::SeparationDistanceConstraint(
	ConstraintPtr constraint,
	InterpolatorPtr interpolator) :
	Constraint(constraint->getType())
{
	constraint_ = constraint;
	interpolator_ = interpolator;
	verbose_ = false;

	separation_distance_ = std::make_shared<double>();

	interpolator->setInput(separation_distance_);

	robot_position_ = std::make_shared<Vector6d>(Vector6d::Zero());
}

SeparationDistanceConstraint::SeparationDistanceConstraint(
	ConstraintPtr constraint,
	InterpolatorPtr interpolator,
	Vector6dConstPtr robot_position) :
	SeparationDistanceConstraint(constraint, interpolator)
{
	robot_position_ = robot_position;
}

double SeparationDistanceConstraint::compute() {
	*separation_distance_ = closestObjectDistance();
	interpolator_->compute();
	return constraint_->compute();
}

double SeparationDistanceConstraint::closestObjectDistance() {
	const Vector3d& rob_pos = robot_position_->block<3,1>(0,0);

	double min_dist = std::numeric_limits<double>::infinity();
	for(const auto& object : objects_) {
		Vector3d obj_rob_vec = object.second->block<3,1>(0,0) - rob_pos;

		min_dist = std::min(min_dist, obj_rob_vec.norm());
	}

	return min_dist;
}