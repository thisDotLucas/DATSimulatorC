#include "DATSimulatorC.h"
#include <systemc.h>
#include <chrono>
#include <thread>

class MotorController : sc_module
{
public:
    SC_CTOR(MotorController)
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
            wait();
            std::cout << "@ " << sc_time_stamp().to_seconds() << "s - Angle: " << m_angleInPort->read() << "\n";
        }
    }

    void readMotorTorquePort(void)
    {
        while (true)
        {
            wait();
            std::cout << "@ " << sc_time_stamp().to_seconds() << "s - Torque: " << m_torqueInPort->read() << "\n";
        }
    }

    void WriteToAngleOutPort(void)
    {
        while (true)
        {
            m_angleOutPort->write((((double)rand() / (double)RAND_MAX)) * 180);

            wait(100, SC_MS);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    void WriteToTorqueOutPort(void)
    {
        while (true)
        {
            m_toqueOutPort->write((((double)rand() / (double)RAND_MAX)) * 180);

            wait(100, SC_MS);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    sc_port<sc_signal_in_if<double>> m_angleInPort;
    sc_port<sc_signal_in_if<double>> m_torqueInPort;

    sc_port<sc_signal_out_if<double>> m_angleOutPort;
    sc_port<sc_signal_out_if<double>> m_toqueOutPort;
};

class Motor : sc_module
{
public:
    SC_CTOR(Motor)
    {
        SC_THREAD(WriteToAngleOutPort);
        SC_THREAD(WriteToTorqueOutPort);

        SC_THREAD(readAnglePort);
        sensitive << m_angleInPort;

        SC_THREAD(readTorquePort);
        sensitive << m_torqueInPort;
    }

    void WriteToAngleOutPort(void)
    {
        while (true) 
        {
            m_angleOutPort->write(m_angle);

            wait(100, SC_MS);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    void WriteToTorqueOutPort(void)
    {
        while (true)
        {
            m_toqueOutPort->write(m_torque);

            wait(100, SC_MS);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    void readAnglePort(void)
    {
        while (true)
        {
            wait();
            m_angle = m_angleInPort->read();
        }
    }

    void readTorquePort(void)
    {
        while (true)
        {
            wait();
            m_torque = m_torqueInPort->read();
        }
    }

    double m_angle{};
    double m_torque{};

    sc_port<sc_signal_out_if<double>> m_angleOutPort;
    sc_port<sc_signal_out_if<double>> m_toqueOutPort;

    sc_port<sc_signal_in_if<double>> m_angleInPort;
    sc_port<sc_signal_in_if<double>> m_torqueInPort;
};

int sc_main(int, char* [])
{
    srand((unsigned)time(NULL));

    MotorController motorController("MotorController");
    Motor motor("Motor");

    sc_signal<double> angleSignalToMotor;
    motorController.m_angleOutPort(angleSignalToMotor);
    motor.m_angleInPort(angleSignalToMotor);

    sc_signal<double> torqueSignalToMotor;
    motorController.m_toqueOutPort(torqueSignalToMotor);
    motor.m_torqueInPort(torqueSignalToMotor);

    sc_signal<double> angleSignalToController;
    motorController.m_angleInPort(angleSignalToController);
    motor.m_angleOutPort(angleSignalToController);

    sc_signal<double> torqueSignalToController;
    motorController.m_torqueInPort(torqueSignalToController);
    motor.m_toqueOutPort(torqueSignalToController);

    sc_start(60 * 10, SC_SEC);

    return 0;
}
