#pragma once

#include <OpenPHRI/robot.h>
#include <OpenPHRI/safety_controller.h>
#include <OpenPHRI/utilities/robot_model.h>
#include <OpenPHRI/utilities/data_logger.h>
#include <OpenPHRI/utilities/exceptions.h>
#include <OpenPHRI/drivers/driver.h>

#include <yaml-cpp/yaml.h>

#include <string>
#include <functional>
#include <memory>

namespace phri {

class AppMaker {
public:
    AppMaker(const std::string& configuration_file);
    ~AppMaker();

    bool
    init(std::function<bool(void)> init_code = std::function<bool(void)>());

    bool run(std::function<bool(void)> pre_controller_code =
                 std::function<bool(void)>(),
             std::function<bool(void)> post_controller_code =
                 std::function<bool(void)>());

    bool stop();

    RobotPtr getRobot() const;
    SafetyControllerPtr getController() const;
    RobotModelPtr getModel() const;
    DriverPtr getDriver() const;
    DataLoggerPtr getDataLogger() const;

    template <typename T> T getParameter(const std::string& name) const {
        const auto& params = getParameters();
        if (params) {
            return getParameters()[name].as<T>();
        } else {
            throw std::runtime_error(OPEN_PHRI_ERROR(
                "there is no 'parameters' field in the configuration file."));
        }
    }

    template <typename T>
    T getParameter(const std::string& name, const T& default_value) const {
        try {
            return getParameters()[name].as<T>(default_value);
        } catch (...) {
            return default_value;
        }
    }

private:
    const YAML::Node& getParameters() const;

    struct pImpl;
    std::unique_ptr<pImpl> impl_;
};

using AppMakerPtr = std::shared_ptr<AppMaker>;
using AppMakerConstPtr = std::shared_ptr<const AppMaker>;

} // namespace phri
