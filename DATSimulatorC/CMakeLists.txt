# CMakeList.txt : CMake project for DATSimulatorC, include source and define
# project specific logic here.
#
find_package(SystemCLanguage CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

set (CMAKE_CXX_STANDARD ${SystemC_CXX_STANDARD})

# Add source to this project's executable.
add_executable(DATSimulatorC DATSimulatorC.cpp DATSimulatorC.h JsonConfig.cpp JsonConfig.h)

target_link_libraries(DATSimulatorC PRIVATE SystemC::systemc)
target_link_libraries(DATSimulatorC PRIVATE nlohmann_json::nlohmann_json)

add_subdirectory(Tests)