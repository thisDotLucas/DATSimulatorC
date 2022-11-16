#include <string>
#include <optional>
#include <vector>

namespace json
{
	struct TimeValuePair
	{
		TimeValuePair(const double time, const double value) : m_time(time), m_value(value) {}
		
		double m_time;
		double m_value;

		bool operator < (const TimeValuePair other) const
		{
			return m_time < other.m_time;
		}
	};

	class JSONConfig
	{
	public:
		JSONConfig(const std::string& path);

		std::optional<double> getSimulationTime() const; // Seconds
		std::optional<double> getStepSize() const; // Milliseconds
		std::optional<double> getAngleAt(const double time) const; // Degrees
		std::optional<double> getTourqueAt(const double time) const;

	private:

		std::optional<double> getValueAt(const std::vector<TimeValuePair>& pairs, const double time) const;

		std::string m_path;
		std::optional<double> m_simulationTime;
		std::optional<double> m_stepSize;
		std::vector<TimeValuePair> m_angles;
		std::vector<TimeValuePair> m_torques;
	};
}
