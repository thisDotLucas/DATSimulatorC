#include "JsonConfig.h"
#include <algorithm>
#include <fstream>
#include <cassert>
#include <nlohmann/json.hpp>

constexpr char* SIMULATION_TIME = "SimulationTime";
constexpr char* STEP_SIZE = "StepSize";
constexpr char* ANGLES = "Angles";
constexpr char* TORQUES = "Torques";
constexpr char* VALUE = "Value";
constexpr char* TIME = "Time";

json::JSONConfig::JSONConfig(const std::string& path) : m_path(path)
{
	std::ifstream file(path);
	if (!file)
		return;

	nlohmann::json data = nlohmann::json::parse(file);
	if (data.empty())
		return;

	auto getNumber = [](nlohmann::json json, const std::string& element) -> std::optional<double>
	{
		return (json.contains(element) && json[element].is_number()) ?
			std::make_optional(json[element]) : std::nullopt;
	};
	
	auto getTimeValuePair = [&getNumber](nlohmann::json json) -> std::optional<json::TimeValuePair>
	{
		std::optional<double> value = getNumber(json, VALUE);
		std::optional<double> time = getNumber(json, TIME);
		return (value.has_value() && time.has_value()) ?
			std::make_optional(json::TimeValuePair{ time.value(), value.value() }) : std::nullopt;
	};

	auto getTimeValuePairs = [&](nlohmann::json json, const std::string& element) -> std::vector<json::TimeValuePair>
	{
		std::vector<json::TimeValuePair> timeValuePairs;
		if (json.contains(element) && json[element].is_array())
		{
			for (const auto& item : json[element].items())
			{
				auto timeValuePair = getTimeValuePair(item.value());
				if (timeValuePair.has_value())
					timeValuePairs.push_back({ timeValuePair->m_time, timeValuePair->m_value });
			}
		}

		std::sort(std::begin(timeValuePairs), std::end(timeValuePairs));

		return timeValuePairs;
	};

	m_simulationTime = getNumber(data, SIMULATION_TIME);
	m_stepSize = getNumber(data, STEP_SIZE);

	m_angles = getTimeValuePairs(data, ANGLES);
	m_torques = getTimeValuePairs(data, TORQUES);
}

std::optional<double> json::JSONConfig::getSimulationTime() const
{
	return m_simulationTime;
}

std::optional<double> json::JSONConfig::getStepSize() const
{
	return m_stepSize;
}

std::optional<double> json::JSONConfig::getAngleAt(const double time) const
{
	return getValueAt(m_angles, time);
}

std::optional<double> json::JSONConfig::getTourqueAt(const double time) const
{
	return getValueAt(m_torques, time);
}

std::optional<double> json::JSONConfig::getValueAt(const std::vector<TimeValuePair>& pairs, const double time) const
{
	assert(std::is_sorted(std::begin(pairs), std::end(pairs)));

	auto it_value = std::find_if(std::begin(pairs), std::end(pairs),
		[&time](const json::TimeValuePair& p) { return p.m_time > time; });

	return it_value != std::begin(pairs) && pairs.size() ?
		std::make_optional(std::prev(it_value)->m_value) : std::nullopt;
}
