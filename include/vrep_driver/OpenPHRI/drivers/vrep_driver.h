/*      File: vrep_driver.h
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

/**
 * @file vrep_driver.h
 * @author Benjamin Navarro
 * @brief Definition of the VREPDriver class
 * @date April 2017
 * @ingroup VREP
 */

/** @defgroup VREP
 * Provides an easy interface to the V-REP simulator
 *
 * Usage: #include <OpenPHRI/drivers/vrep_driver.h>
 *
 */

#pragma once

#include <string>
#include <unordered_map>
#include <map>
#include <vector>
#include <utility>

#include <OpenPHRI/definitions.h>
#include <OpenPHRI/robot.h>
#include <OpenPHRI/fwd_decl.h>
#include <OpenPHRI/drivers/driver.h>

namespace phri {

/** @brief Wrapper for V-REP low level C API.
 *  @details Can be used to control the simulation (start/pause/stop), to tack
 * objects' position, read and send robot related data. Some objects need to be
 * present in the scene: tcp (actual TCP pose), tcp_target (TCP reference pose),
 * force_sensor (force sensor attached to the TCP), base_frame (frame attached
 * to the base link) and world_frame (frame attached to the environment). All
 * these objects can have a prefix and/or suffix specified in the constructor.
 */
class VREPDriver : virtual public phri::Driver {
public:
    /**
     * @brief Construct a driver using an IP & port. Prefix and suffix can be
     * used to target a specific robot.
     * @param robot The robot to read/write data from/to.
     * @param sample_time The sample time set in V-REP (sorry, no way to get it
     * remotely...).
     * @param suffix [optional] Can be used to specify a suffix for all the
     * basic objects.
     * @param ip [optional] Can be used to specify a IP address different than
     * the local network.
     * @param port [optional] Can be used to specify a port different thant the
     * default one.
     */
    VREPDriver(phri::Robot& robot, double sample_time,
               const std::string& suffix = "",
               const std::string& ip = "127.0.0.1", int port = 19997);

    /**
     * @brief Construct a driver using an already open connection. Prefix and
     * suffix can be used to target a specific robot.
     * @param robot The robot to read/write data from/to.
     * @param sample_time The sample time set in V-REP (sorry, no way to get it
     * remotely...).
     * @param client_id The client ID of the previously openned connection.
     * @param suffix [optional] Can be used to specify a suffix for all the
     * basic objects.
     */
    VREPDriver(phri::Robot& robot, double sample_time, int client_id,
               const std::string& suffix = "");

    VREPDriver(phri::Robot& robot, const YAML::Node& configuration);

    virtual ~VREPDriver();

    /**
     * Initialize the communication with V-REP
     * @param timeout The maximum time to wait to establish the connection.
     * @return true on success, false otherwise
     */
    bool init(double timeout = 5.) override;

    /**
     * @brief Retrieve the client ID of the current connection.
     * @return The client ID. A negative value means that the connection has
     * failed to open.
     */
    [[nodiscard]] int getClientID() const;

    /**
     * @brief Check the state of the connection.
     * @return True if the connection is still open, false otherwise.
     */
    [[nodiscard]] bool checkConnection() const;

    /**
     * @brief Turn synchronous operation on or off. When on,
     * VREPDriver::nextStep has to be called to trigger a simulation step.
     * @return True if correctly set, false otherwise.
     */
    bool enableSynchonous(bool state);

    /**
     * @brief Trigger a simulation step.
     * @return True if correctly set, false otherwise or if synchronous
     * operation is disabled.
     */
    bool nextStep() const;

    /**
     * @brief see nextStep.
     */
    bool sync() override;

    /**
     * @brief Start the simulation.
     */
    bool start(double timeout = 0.) override;

    /**
     * @brief Stop the simulation.
     */
    bool stop() override;

    /**
     * @brief Pause the simulation.
     */
    void pause();

    /**
     * @brief Get the TCP pose in the given frame.
     * @param pose [out] The current TCP pose.
     * @return True if correctly read, false otherwise.
     */
    bool readTCPPose(spatial::Position& pose) const;

    /**
     * @brief Get the TCP velocity in the given frame.
     * @param velocity [out] The current TCP velocity.
     * @param frame [in] The reference frame.
     * @return True if correctly read, false otherwise.
     */
    bool readTCPVelocity(spatial::Velocity& velocity) const;

    /**
     * @brief Get the TCP wrench in the TCP frame.
     * @param wrench [out] The current TCP wrench.
     * @return True if correctly read, false otherwise.
     */
    bool readTCPWrench(spatial::Force& wrench) const;

    /**
     * @brief Start tracking a given object in the specified frame.
     * @param name [in] The full name of the object to track.
     * @param frame [in] The reference frame.
     * @return A shared pointer to the object's postion. Updated on
     * VREPDriver::updateTrackedObjectsPosition.
     */
    [[nodiscard]] std::shared_ptr<const spatial::Position>
    trackObjectPosition(const std::string& name, const spatial::Frame& frame);

    /**
     * @brief Update all tracked objects' position.
     * @return True if correctly read, false otherwise.
     */
    bool updateTrackedObjectsPosition();

    [[nodiscard]] std::shared_ptr<const vector::dyn::Position>
    initLaserScanner(const std::string& name);

    bool updateLaserScanners();

    bool readJointPosition(vector::dyn::Position& position);
    bool sendJointTargetPosition(const vector::dyn::Position& position);
    bool sendJointTargetVelocity(const vector::dyn::Velocity& velocity);

    bool read() override;
    bool send() override;

    [[nodiscard]] static bool isRegisteredInFactory();

    [[nodiscard]] static constexpr spatial::Frame worldFrame() {
        return spatial::Frame{"world"};
    }

    void setScene(const std::string& path);

private:
    // void init(const std::string& ip, int port);
    // void init(int client_id);
    bool getObjectHandles();
    void startStreaming() const;
    [[nodiscard]] int getFrameHandle(const spatial::Frame& frame) const;
    void loadScene(const std::string& path) const;

    struct StartParameters {
        int client_id = -1;
        std::string ip;
        int port;
        std::string scene_to_load;
    };

    bool sync_mode_;
    std::string suffix_;
    int client_id_;
    StartParameters start_parameters_;

    std::unordered_map<std::string, int> object_handles_;
    std::map<std::string, std::shared_ptr<vector::dyn::Position>> lasers_data_;
    std::map<std::pair<int, int>, std::shared_ptr<spatial::Position>>
        tracked_objects_;

    static const bool registered_in_factory;
    static std::map<std::string, int> connection_to_client_id;
};

} // namespace phri
