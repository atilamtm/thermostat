#include <gtest/gtest.h>
#include "../src/thermostat.h"
#include "../src/test/mock_thermometer.h"
#include "../src/test/mock_temperature_controller.h"

using ::testing::AtLeast;
using ::testing::Return;
using ::testing::_;


// Demonstrate happy scenario.
TEST(ThermostatUnit, Happy) {
    MockThermometer meter;
    MockTemperatureController controller;

    EXPECT_CALL(meter, SetTemperatureThresholds(_,_))
        .Times(AtLeast(1))
        .WillOnce(Return(true)); 
    EXPECT_CALL(meter, RegisterCallback(_))
        .Times(AtLeast(1)); 
    EXPECT_CALL(meter, GetTemperature())
        .Times(AtLeast(1))
        .WillOnce(Return(20)); 

    Thermostat stat(meter, controller); 
  // Expect two strings not to be equal
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

