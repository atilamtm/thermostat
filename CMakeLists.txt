cmake_minimum_required(VERSION 3.14)

project(thermostat)

# GoogleTest requires at least C++20 for use for bind_front
set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/b796f7d44681514f58a683a3a71ff17c94edb0c1.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
set(BUILD_GMOCK ON)

add_executable(
  thermostat_test
  src/thermometer.h
  src/temperature_controller.h
  src/test/mock_thermometer.h
  src/test/mock_temperature_controller.h
  test/thermostat_test.cc
)

target_link_libraries(
  thermostat_test
  GTest::gtest_main
  GTest::gmock_main
)

include(GoogleTest)
gtest_discover_tests(thermostat_test)

