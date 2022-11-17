#include <gtest/gtest.h>
#include <filesystem>
#include "../JsonConfig.h"

constexpr double TEST_SIMULATION_TIME = 200.0;
constexpr double TEST_STEP_SIZE = 100.0;

namespace
{
	std::string path()
	{
		return std::string(TESTS_DIR) + "/test.json";
	}
}

TEST(JSONConfigParser, parseSimulationTime)
{
	json::JSONConfig config(path());
	ASSERT_TRUE(config.getSimulationTime() == TEST_SIMULATION_TIME);
}

TEST(JSONConfigParser, parseStepSize)
{
	json::JSONConfig config(path());
	ASSERT_TRUE(config.getStepSize() == TEST_STEP_SIZE);
}

TEST(JSONConfigParser, parseAngles)
{
	json::JSONConfig config(path());
	ASSERT_TRUE(config.getAngleAt(-1.0) == std::nullopt);
	ASSERT_TRUE(config.getAngleAt(0 - std::numeric_limits<double>::min()) == std::nullopt);
	ASSERT_TRUE(config.getAngleAt(0.0) == 90);
	ASSERT_TRUE(config.getAngleAt(2.5) == 90);
	ASSERT_TRUE(config.getAngleAt(5.0) == 60);
	ASSERT_TRUE(config.getAngleAt(7.2) == 60);
	ASSERT_TRUE(config.getAngleAt(10.0) == 30);
	ASSERT_TRUE(config.getAngleAt(20.2) == 30);
	ASSERT_TRUE(config.getAngleAt(35.0) == 50);
	ASSERT_TRUE(config.getAngleAt(100.0) == 50);
	ASSERT_TRUE(config.getAngleAt(std::numeric_limits<double>::max()) == 50);
}

TEST(JSONConfigParser, parseTorques)
{
	json::JSONConfig config(path());
	ASSERT_TRUE(config.getTourqueAt(-1.0) == std::nullopt);
	ASSERT_TRUE(config.getTourqueAt(0 - std::numeric_limits<double>::min()) == std::nullopt);
	ASSERT_TRUE(config.getTourqueAt(0.0) == 20);
	ASSERT_TRUE(config.getTourqueAt(2.5) == 20);
	ASSERT_TRUE(config.getTourqueAt(5.0) == 40);
	ASSERT_TRUE(config.getTourqueAt(7.2) == 40);
	ASSERT_TRUE(config.getTourqueAt(50.0) == 80);
	ASSERT_TRUE(config.getTourqueAt(100.0) == 80);
	ASSERT_TRUE(config.getTourqueAt(std::numeric_limits<double>::max()) == 80);
}

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}