#include "DATSimulatorC.h"
#include "JsonConfig.h"
#include <systemc>
#include <chrono>
#include <thread>
#include <iomanip>
#include <sstream>
#include <fstream>

constexpr double DEFAULT_STEP_SIMULATION_TIME = 60 * 10;
constexpr double DEFAULT_STEP_SIZE = 100;

using namespace sc_core;

namespace
{
    std::optional<std::string> isValidPath(const std::string& path)
    {
        return std::ifstream(path) ? std::make_optional(path) : std::nullopt;
    }

    std::optional<std::string> getPath()
    {
    #ifdef IS_DEBUG
        // You can set the path to a JSON file here to not be prompted while debugging.
        std::optional<std::string> debugPath = "C:/Users/lucas/Downloads/new.json";
        if (debugPath.has_value())
            return isValidPath(debugPath.value());
    #endif

        std::string path;
        std::cout << "Path for config file: ";
        std::getline(std::cin, path);

        return isValidPath(path);
    }

    template <typename T>
    std::string logValue(const sc_time& timeInSimulation, const std::string& attribute, const T value)
    {
        std::stringstream ss{};
        ss << std::setprecision(2) << std::fixed << "@ " << timeInSimulation.to_seconds() << "s - " << attribute + ": " << value;
        return ss.str();
    }
}

class MotorController : public sc_module
{
public:
    MotorController(sc_module_name name, const json::JSONConfig& config) : sc_module(name), m_config(config)
    {
        SC_THREAD(readMotorAnglePort);
        sensitive << m_angleInPort;

        SC_THREAD(readMotorTorquePort);
        sensitive << m_torqueInPort;

        SC_THREAD(WriteToAngleOutPort);
        SC_THREAD(WriteToTorqueOutPort);
    }

    void readMotorAnglePort(void)
    {
        while (true)
        {
            std::cout << logValue<double>(sc_time_stamp(), "Angle", m_angleInPort->read()) << "\n";
            wait();
        }
    }

    void readMotorTorquePort(void)
    {
        while (true)
        {
            std::cout << logValue<double>(sc_time_stamp(), "Torque", m_torqueInPort->read()) << "\n";
            wait();
        }
    }

    void WriteToAngleOutPort(void)
    {
        while (true)
        {
            m_angleOutPort->write(m_config.getAngleAt(sc_time_stamp().to_seconds()).value_or(0.0));
            wait(m_config.getStepSize().value_or(DEFAULT_STEP_SIZE), SC_MS);
        }
    }

    void WriteToTorqueOutPort(void)
    {
        while (true)
        {
            m_torqueOutPort->write(m_config.getTourqueAt(sc_time_stamp().to_seconds()).value_or(0.0));
            wait(m_config.getStepSize().value_or(DEFAULT_STEP_SIZE), SC_MS);
        }
    }

    sc_port<sc_signal_in_if<double>> m_angleInPort;
    sc_port<sc_signal_in_if<double>> m_torqueInPort;

    sc_port<sc_signal_out_if<double>> m_angleOutPort;
    sc_port<sc_signal_out_if<double>> m_torqueOutPort;

    json::JSONConfig m_config;

    SC_HAS_PROCESS(MotorController);
};

class Motor : public sc_module
{
public:
    Motor(sc_module_name name, const json::JSONConfig& config) : sc_module(name), m_config(config)
    {
        SC_THREAD(WriteToAngleOutPort);
        SC_THREAD(WriteToTorqueOutPort);

        SC_THREAD(readAnglePort);
        sensitive << m_angleInPort;
        dont_initialize();

        SC_THREAD(readTorquePort);
        sensitive << m_torqueInPort;
        dont_initialize();
    }

    void WriteToAngleOutPort(void)
    {
        while (true) 
        {
            m_angleOutPort->write(m_angle);
            wait(m_config.getStepSize().value_or(DEFAULT_STEP_SIZE), SC_MS);
        }
    }

    void WriteToTorqueOutPort(void)
    {
        while (true)
        {
            m_torqueOutPort->write(m_torque);
            wait(m_config.getStepSize().value_or(DEFAULT_STEP_SIZE), SC_MS);
        }
    }

    void readAnglePort(void)
    {
        while (true)
        {
            m_angle = m_angleInPort->read();
            wait();
        }
    }

    void readTorquePort(void)
    {
        while (true)
        {
            m_torque = m_torqueInPort->read();
            wait();
        }
    }

    double m_angle{};
    double m_torque{};

    sc_port<sc_signal_out_if<double>> m_angleOutPort;
    sc_port<sc_signal_out_if<double>> m_torqueOutPort;

    sc_port<sc_signal_in_if<double>> m_angleInPort;
    sc_port<sc_signal_in_if<double>> m_torqueInPort;

    json::JSONConfig m_config;
    
    SC_HAS_PROCESS(Motor);
};

int sc_main(int, char* [])
{
    auto path = getPath();
    if (!path.has_value())
        return 0;

    json::JSONConfig config(path.value());

    MotorController motorController("MotorController", config);
    Motor motor("Motor", config);

    sc_signal<double> angleSignalToController;
    motorController.m_angleInPort(angleSignalToController);
    motor.m_angleOutPort(angleSignalToController);

    sc_signal<double> torqueSignalToController;
    motorController.m_torqueInPort(torqueSignalToController);
    motor.m_torqueOutPort(torqueSignalToController);

    sc_signal<double> angleSignalToMotor;
    motorController.m_angleOutPort(angleSignalToMotor);
    motor.m_angleInPort(angleSignalToMotor);

    sc_signal<double> torqueSignalToMotor;
    motorController.m_torqueOutPort(torqueSignalToMotor);
    motor.m_torqueInPort(torqueSignalToMotor);
    
    sc_start(config.getSimulationTime().value_or(DEFAULT_STEP_SIMULATION_TIME), SC_SEC);

    return 0;
}
