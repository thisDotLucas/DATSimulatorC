set(SystemCLanguage_DIR $ENV{VCPKG_ROOT}/packages/systemc_$ENV{VCPKG_DEFAULT_TRIPLET}/share/systemclanguage)

find_package(SystemCLanguage CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)

set (CMAKE_CXX_STANDARD ${SystemC_CXX_STANDARD})

add_compile_definitions(TESTS_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

# Add source to this project's executable.
add_executable(DATSimulatorCTest ../JsonConfig.cpp ../JsonConfig.h tests.cpp)

target_link_libraries(DATSimulatorCTest PRIVATE SystemC::systemc)
target_link_libraries(DATSimulatorCTest PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(DATSimulatorCTest PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)

# TODO: Add tests and install targets if needed.
enable_testing()
add_test(tests DATSimulatorCTest)