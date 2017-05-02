#include <RSCL/utilities/trajectory_generator.h>
#include <iostream>

using namespace RSCL;

Trajectory::Trajectory(TrajectoryOutputType output_type, TrajectoryPointPtr start, doublePtr output, double sample_time) :
	total_minimum_time_(0.),
	current_section_(0),
	sample_time_(sample_time),
	output_type_(output_type)
{
	points_.push_back(start);
	output_ = output;
}

Trajectory::Trajectory(TrajectoryOutputType output_type, TrajectoryPointPtr start, double* output, double sample_time) :
	Trajectory(output_type, start, std::shared_ptr<double>(output, [](auto p){}), sample_time)
{
}

Trajectory::Trajectory(TrajectoryOutputType output_type, TrajectoryPointPtr start, double sample_time) :
	Trajectory(output_type, start, std::make_shared<double>(0.), sample_time)
{
}

void Trajectory::addPathTo(TrajectoryPointPtr to, double max_velocity, double max_acceleration) {
	points_.push_back(to);
	PathParams params;
	params.max_velocity = max_velocity;
	params.max_acceleration = max_acceleration;
	params.isFixedTime = false;
	path_params_.push_back(params);
}

void Trajectory::addPathTo(TrajectoryPointPtr to, double duration) {
	points_.push_back(to);
	PathParams params;
	params.minimum_time = duration;
	params.isFixedTime = true;
	path_params_.push_back(params);
}

doubleConstPtr Trajectory::getOutput() {
	return output_;
}

double Trajectory::getCurrentPathMinimumTime() {
	return path_params_[current_section_].minimum_time;
}

double Trajectory::getTrajectoryMinimumTime() {
	return total_minimum_time_;
}

double Trajectory::getPathMinimumTime(size_t idx) {
	if(idx < points_.size()-1) {
		return path_params_[idx].minimum_time;
	}
	else {
		return 0.;
	}
}

size_t Trajectory::getSegmentCount() {
	return points_.size()-1;
}

void Trajectory::setPaddingTime(size_t idx, double time) {
	if(idx < points_.size()-1) {
		path_params_[idx].padding_time = time;
	}
}

void Trajectory::setPathCurrentTime(size_t idx, double time) {
	if(idx < points_.size()-1) {
		path_params_[idx].current_time = time;
	}
}

bool Trajectory::computeParameters() {
	int segments = points_.size()-1;
	if(segments < 1) {
		return false;
	}

	for (size_t i = 0; i < segments; ++i) {
		TrajectoryPoint& from = *points_[i];
		TrajectoryPoint& to = *points_[i+1];

		auto& params = path_params_[i];
		auto& poly_params = params.poly_params;

		poly_params = {0, params.minimum_time + params.padding_time, *from.y, *to.y, *from.dy, *to.dy, *from.d2y, *to.d2y};
		FifthOrderPolynomial::computeParameters(poly_params);

		params.current_time = 0.;
	}

	return true;
}

bool Trajectory::computeTimings(double v_eps, double a_eps) {
	int segments = points_.size()-1;
	if(segments < 1) {
		return false;
	}

	total_minimum_time_ = 0.;

	for (size_t i = 0; i < segments; ++i) {
		auto& params = path_params_[i];
		if(not params.isFixedTime) {
			TrajectoryPoint& from = *points_[i];
			TrajectoryPoint& to = *points_[i+1];

			auto& poly_params = params.poly_params;
			poly_params = {0, 1., *from.y, *to.y, *from.dy, *to.dy, *from.d2y, *to.d2y};

			bool vmax_found = false;
			bool amax_found = false;
			while(not (vmax_found and amax_found)) {
				FifthOrderPolynomial::computeParameters(poly_params);

				if(not vmax_found) {
					double v_max = std::abs(FifthOrderPolynomial::getFirstDerivativeMaximum(poly_params));
					double v_error = v_max - params.max_velocity;
					double v_error_abs = std::abs(v_error);
					vmax_found = v_error_abs < v_eps;
					if(not vmax_found) {
						poly_params.xf += v_error/params.max_velocity;
					}
				}
				else if(not amax_found) {
					double a_max = std::abs(FifthOrderPolynomial::getSecondDerivativeMaximum(poly_params));
					double a_error = a_max - params.max_acceleration;
					amax_found = a_error < a_eps;
					if(not amax_found) {
						poly_params.xf += std::sqrt(a_error/params.max_acceleration);
					}
				}

			}

			params.minimum_time = poly_params.xf;
		}

		total_minimum_time_ += params.minimum_time;
	}

	return true;
}

bool Trajectory::compute() {
	if(current_section_ == points_.size()-1) {
		return true;
	}

	auto& params = path_params_[current_section_];

	double dt = (params.minimum_time + params.padding_time) - params.current_time;
	if(dt < 0.) {
		++current_section_;
		setPathCurrentTime(current_section_, -dt);
	}

	switch(output_type_) {
	case TrajectoryOutputType::Position:
		*output_ = FifthOrderPolynomial::compute(params.current_time, params.poly_params);
		break;
	case TrajectoryOutputType::Velocity:
		*output_ = FifthOrderPolynomial::computeFirstDerivative(params.current_time, params.poly_params);
		break;
	case TrajectoryOutputType::Acceleration:
		*output_ = FifthOrderPolynomial::computeSecondDerivative(params.current_time, params.poly_params);
		break;
	}

	params.current_time += sample_time_;

	return false;
}



TrajectoryGenerator::TrajectoryGenerator(TrajectorySynchronization sync)
{
	setSynchronizationMethod(sync);
}

void TrajectoryGenerator::setSynchronizationMethod(TrajectorySynchronization sync) {
	sync_ = sync;
}

void TrajectoryGenerator::computeParameters() {
	for(auto& traj: items_) {
		traj.second->computeTimings();
	}

	if(sync_ == TrajectorySynchronization::SynchronizeWaypoints) {
		size_t max_segments = 0;
		for(auto& traj: items_) {
			max_segments = std::max(max_segments, traj.second->getSegmentCount());
		}

		for (size_t idx = 0; idx < max_segments; ++idx) {
			double max_time = 0.;
			for(auto& traj: items_) {
				max_time = std::max(max_time, traj.second->getPathMinimumTime(idx));
			}
			for(auto& traj: items_) {
				traj.second->setPaddingTime(idx, max_time - traj.second->getPathMinimumTime(idx));
			}
		}
	}
	else if(sync_ == TrajectorySynchronization::SynchronizeTrajectory) {
		double max_time = 0.;
		for(auto& traj: items_) {
			max_time = std::max(max_time, traj.second->getTrajectoryMinimumTime());
		}
		for(auto& traj: items_) {
			size_t segments = traj.second->getSegmentCount();
			double padding = (max_time - traj.second->getTrajectoryMinimumTime())/double(segments);
			for (size_t idx = 0; idx < segments; ++idx) {
				traj.second->setPaddingTime(idx, padding);
			}
		}
	}
	else {
		for(auto& traj: items_) {
			for (size_t idx = 0; idx < traj.second->getSegmentCount(); ++idx) {
				traj.second->setPaddingTime(idx, 0.);
			}
		}
	}

	for(auto& traj: items_) {
		traj.second->computeParameters();
	}
}

bool TrajectoryGenerator::compute() {
	bool all_ok = true;

	for(auto& traj: items_) {
		all_ok &= traj.second->compute();
	}

	return all_ok;
}