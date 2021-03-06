from minieigen import *
from openphri import *
from math import *
import sys

def isClose(v1, v2, eps = 1e-3):
	return abs(v1-v2) < eps

def getTransVel(vec):
	t = Vector3.Zero
	for i in range(0,3):
		t[i] = vec[i]
	return t

def power(velocity, force):
	return force.dot(velocity)

damping_matrix = NewMatrix6dPtr(Matrix6.Identity)
safety_controller = NewSafetyController(damping_matrix)
safety_controller.setVerbose(True)

tcp_velocity = safety_controller.getTCPVelocity()
total_velocity = safety_controller.getTotalVelocity()

external_force = NewVector6dPtr()

maximum_power = NewDoublePtr(10)
power_constraint = NewPowerConstraint(external_force, maximum_power)

constant_vel = NewVector6dPtr()
constant_velocity_generator = NewVelocityProxy(constant_vel)

safety_controller.addConstraint("power constraint", power_constraint)
safety_controller.addVelocityGenerator("vel proxy", constant_velocity_generator)

# Step #1 : no velocity
safety_controller.updateTCPVelocity()

assert_msg("Step #1", isClose(power(tcp_velocity, external_force), 0.))

# Step #2 : velocity 1 axis, no force
constant_vel[0] = 0.2
safety_controller.updateTCPVelocity()

assert_msg("Step #2", isClose(power(tcp_velocity, external_force), 0.))

# Step #3 : velocity 1 axis, force same axis with opposite sign < max
external_force[0] = -10.
safety_controller.updateTCPVelocity()

assert_msg("Step #3", isClose(power(tcp_velocity, external_force), -2.))

# Step #4 : velocity 1 axis, force same axis with same sign < max
external_force[0] = 10.
safety_controller.updateTCPVelocity()

assert_msg("Step #4", isClose(power(tcp_velocity, external_force), 2.))

# Step #5 : velocity 1 axis, force same axis with opposite sign > max
external_force[0] = -100.
safety_controller.updateTCPVelocity()

assert_msg("Step #5", isClose(power(tcp_velocity, external_force), -maximum_power.get()))

# Step #6 : velocity 1 axis, force same axis with same sign > max
external_force[0] = 100.
safety_controller.updateTCPVelocity()

assert_msg("Step #6", tcp_velocity.isApprox(total_velocity))

sys.exit(0)
