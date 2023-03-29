#include <gtest/gtest.h>
#include "../src/thermostat.h"
#include "../src/test/mock_thermometer.h"
#include "../src/test/mock_temperature_controller.h"

using ::testing::AtLeast;
using ::testing::Return;
using ::testing::_;
using ::testing::InSequence;


// Demonstrate happy scenario.
TEST(ThermostatUnit, Happy) {
    MockThermometer meter;
    MockTemperatureController controller;

    // Calls from thermostat constructor
    EXPECT_CALL(meter, SetTemperatureThresholds(_,_))
        .Times(1)
        .WillOnce(Return(true)); 
    EXPECT_CALL(meter, RegisterCallback(_))
        .Times(1); 
    EXPECT_CALL(meter, GetTemperature())
        .Times(AtLeast(1))
        .WillOnce(Return(20)); 
    EXPECT_CALL(controller, Heat(false))
        .Times(1); 
    EXPECT_CALL(controller, Cool(false))
        .Times(1); 

    Thermostat stat(meter, controller); 
}

// Initial temperature read as above high threshold
TEST(ThermostatUnit, StartWithHightemp) {
    MockThermometer meter;
    MockTemperatureController controller;

    // Calls from thermostat constructor
    EXPECT_CALL(meter, SetTemperatureThresholds(_,_))
        .Times(1)
        .WillOnce(Return(true)); 
    EXPECT_CALL(meter, RegisterCallback(_))
        .Times(1); 
    EXPECT_CALL(meter, GetTemperature())
        .Times(AtLeast(1))
        .WillOnce(Return(50)); 
    EXPECT_CALL(controller, Heat(false))
        .Times(1); 
    EXPECT_CALL(controller, Cool(true))
        .Times(1); 

    Thermostat stat(meter, controller); 
}

// Initial temperature read as below low threshold
TEST(ThermostatUnit, StartWithLowTemp) {
    MockThermometer meter;
    MockTemperatureController controller;

    // Calls from thermostat constructor
    EXPECT_CALL(meter, SetTemperatureThresholds(_,_))
        .Times(1)
        .WillOnce(Return(true)); 
    EXPECT_CALL(meter, RegisterCallback(_))
        .Times(1); 
    EXPECT_CALL(meter, GetTemperature())
        .Times(AtLeast(1))
        .WillOnce(Return(-10)); 
    EXPECT_CALL(controller, Heat(true))
        .Times(1); 
    EXPECT_CALL(controller, Cool(false))
        .Times(1); 

    Thermostat stat(meter, controller); 
}

// Receive notification when temperature falls below low threshold
TEST(ThermostatUnit, HeatWhenLowTemp) {
    MockThermometer meter;
    MockTemperatureController controller;

    // Constructor
    EXPECT_CALL(meter, SetTemperatureThresholds(_,_))
        .Times(1)
        .WillOnce(Return(true)); 
    EXPECT_CALL(meter, RegisterCallback(_))
        .Times(1); 
    EXPECT_CALL(meter, GetTemperature())
        .Times(AtLeast(1))
        .WillOnce(Return(20));
 
    // Expect 1x call for Heat(false) and Cool(false) (constructor)
    // Expect 1x call for Heat(true) and Cool(false) to heat the room
    EXPECT_CALL(controller, Heat(true))
        .Times(1); 
    EXPECT_CALL(controller, Cool(false))
        .Times(2); 
    EXPECT_CALL(controller, Cool(true))
        .Times(0); 
    EXPECT_CALL(controller, Heat(false))
        .Times(1); 

    Thermostat stat(meter, controller);

    // Simulate thermometer calling the registered callback function indicating temperature is lower than low threshold.
    bool isHigh = false;
    stat.ThermometerCallback(isHigh);
}

// Receive notification when temperature raises above high threshold
TEST(ThermostatUnit, CoolWhenHighTemp) {
    MockThermometer meter;
    MockTemperatureController controller;

    // Constructor
    EXPECT_CALL(meter, SetTemperatureThresholds(_,_))
        .Times(1)
        .WillOnce(Return(true)); 
    EXPECT_CALL(meter, RegisterCallback(_))
        .Times(1); 
    EXPECT_CALL(meter, GetTemperature())
        .Times(AtLeast(1))
        .WillOnce(Return(20));
 
    // Expect 1x call for Heat(false) and Cool(false) (constructor)
    // Expect 1x call for Heat(false) and Cool(true) to Cool the room
    EXPECT_CALL(controller, Heat(true))
        .Times(0); 
    EXPECT_CALL(controller, Cool(false))
        .Times(1); 
    EXPECT_CALL(controller, Cool(true))
        .Times(1); 
    EXPECT_CALL(controller, Heat(false))
        .Times(2); 

    Thermostat stat(meter, controller); 
    
    // Simulate thermometer calling the registered callback function indicating temperature is higher than high threshold.
    bool isHigh = true;
    stat.ThermometerCallback(isHigh);
}

// Change temperature thresholds to new values
TEST(ThermostatUnit, AdjustThresholdsWhenRequested) {
    MockThermometer meter;
    MockTemperatureController controller;

    // Constructor
    EXPECT_CALL(meter, RegisterCallback(_))
        .Times(1); 
    EXPECT_CALL(meter, GetTemperature())
        .Times(AtLeast(1))
        .WillOnce(Return(20));
    EXPECT_CALL(controller, Cool(false))
        .Times(1); 
    EXPECT_CALL(controller, Heat(false))
        .Times(1); 

    // Check that the underlying thermometer calls are made to adjust notification threshold
    EXPECT_CALL(meter, SetTemperatureThresholds(20,15))
        .WillOnce(Return(true)); 
    EXPECT_CALL(meter, SetTemperatureThresholds(10,5))
        .WillOnce(Return(true)); 
    EXPECT_CALL(meter, SetTemperatureThresholds(80,0))
        .WillOnce(Return(true)); 

    // This expectation will be met by the thermostat constructor, not the tests below
    EXPECT_CALL(meter, SetTemperatureThresholds(40,10))
        .WillOnce(Return(true)); 

    Thermostat stat(meter, controller);
    stat.SetTemperatureThresholds(80,0);
    stat.SetTemperatureThresholds(10,5);
    stat.SetTemperatureThresholds(20,15);
}

// Check whether invalid values for threshold configuration are rejected
TEST(ThermostatUnit, InvalidThresholds) {
    MockThermometer meter;
    MockTemperatureController controller;

    // Constructor
    EXPECT_CALL(meter, SetTemperatureThresholds(_,_))
        .Times(1)
        .WillOnce(Return(true)); 
    EXPECT_CALL(meter, RegisterCallback(_))
        .Times(1); 
    EXPECT_CALL(meter, GetTemperature())
        .Times(AtLeast(1))
        .WillOnce(Return(20));
    EXPECT_CALL(controller, Cool(false))
        .Times(1); 
    EXPECT_CALL(controller, Heat(false))
        .Times(1); 

    Thermostat stat(meter, controller); 
    // Limiting the expectation of the numbe of calls to (meter, SetTemperatureThresholds) to 1 ensures that the calls below will be rejected.
    EXPECT_EQ(stat.SetTemperatureThresholds(-5,0), false);
    EXPECT_EQ(stat.SetTemperatureThresholds(10,10), false);
    EXPECT_EQ(stat.SetTemperatureThresholds(5,10), false);
    EXPECT_EQ(stat.SetTemperatureThresholds(20,80), false);
}

// Check that the temperature controller isn't triggered if the thermostat is deisabled
TEST(ThermostatUnit, NoActionWhenThermostatDisabled) {
    MockThermometer meter;
    MockTemperatureController controller;

    // Constructor
    EXPECT_CALL(meter, SetTemperatureThresholds(_,_))
        .Times(1)
        .WillOnce(Return(true)); 
    EXPECT_CALL(meter, RegisterCallback(_))
        .Times(1); 
    EXPECT_CALL(meter, GetTemperature())
        .Times(AtLeast(1))
        .WillOnce(Return(20));
    EXPECT_CALL(controller, Cool(false))
        .Times(1); 
    EXPECT_CALL(controller, Heat(false))
        .Times(1); 

    Thermostat stat(meter, controller);
    stat.EnableThermostat(false);   
 
    // Simulate thermometer calling the registered callback function indicating temperature thresholds breached.
    // Limiting the expected number of calls to the Heat and Cool functions ensures that the calls below will not cause the thermostat to activate the temperature controller.
    bool isHigh = true;
    stat.ThermometerCallback(isHigh);
    stat.ThermometerCallback(isHigh);
    isHigh = false;
    stat.ThermometerCallback(isHigh);
    isHigh = true;
    stat.ThermometerCallback(isHigh);
}

// Check that the thermostat can be reenabled after is was disabled, and the correct actions are taken according to the current temperature.
TEST(ThermostatUnit, ReenableThermostat) {
    MockThermometer meter;
    MockTemperatureController controller;

    // Constructor
    EXPECT_CALL(meter, SetTemperatureThresholds(_,_))
        .Times(1)
        .WillOnce(Return(true)); 
    EXPECT_CALL(meter, RegisterCallback(_))
        .Times(1); 
    // 1x from thermostat constructor - return 20
    // 3x when thermostat is reenabled - return 20, then 50, then -10
    EXPECT_CALL(meter, GetTemperature())
        .Times(4)
        .WillOnce(Return(20))
        .WillOnce(Return(20))
        .WillOnce(Return(50))
        .WillOnce(Return(-10));

    // From constructor, expect Heat(false) and Cool(false) - temperature within low and high thresholds
    // After first thermostat disable-reenable, expect Heat(false) and Cool(false) - temperature within high and low thresholds
    // 1x Notification of temperature rising higher than high threshold - Expect Heat(false) and Cool(True)
    // 1x Notification of temperature falling lower than low  threshold - Expect Heat(true) and Cool(false)
    // After second thermostat disable-reenable, expect Heat(false) and Cool(true) - temperature higher than high threshold
    // After third thermostat disable-reenable, expect Heat(true) and Cool(false) - temperature lower than low threshold
    EXPECT_CALL(controller, Heat(true))
        .Times(2); 
    EXPECT_CALL(controller, Heat(false))
        .Times(4); 
    EXPECT_CALL(controller, Cool(true))
        .Times(2); 
    EXPECT_CALL(controller, Cool(false))
        .Times(4); 

    Thermostat stat(meter, controller);

    // Disable the thermostat
    stat.EnableThermostat(false);   
 
    // Simulate thermometer calling the registered callback function indicating temperature thresholds breached.
    // No calls from thermostat to underlying component should be triggered.
    bool isHigh = true;
    stat.ThermometerCallback(isHigh);
    isHigh = false;
    stat.ThermometerCallback(isHigh);

    // Reenable the thermostat - temperature should be checked and acted upon.
    stat.EnableThermostat(true);   
 
    // Simulate temperature notifications, first lower than low threshold, then higher than high threshold.
    isHigh = false;
    stat.ThermometerCallback(isHigh);
    isHigh = true;
    stat.ThermometerCallback(isHigh);

    // Disable and reenable thermostat
    stat.EnableThermostat(false);   
    stat.EnableThermostat(true);   

    // Disable and reenable thermostat
    stat.EnableThermostat(false);   
    stat.EnableThermostat(true);   
}

// Send several notifications of temperature rising and falling
TEST(ThermostatUnit, HeatAndCoolSequence) {
    MockThermometer meter;
    MockTemperatureController controller;

    // Constructor
    EXPECT_CALL(meter, SetTemperatureThresholds(_,_))
        .Times(1)
        .WillOnce(Return(true)); 
    EXPECT_CALL(meter, RegisterCallback(_))
        .Times(1); 
    EXPECT_CALL(meter, GetTemperature())
        .Times(AtLeast(1))
        .WillOnce(Return(20));

    {
        InSequence seq;

        // Expected from contructor
        EXPECT_CALL(controller, Heat(false));
        EXPECT_CALL(controller, Cool(false));
    
        // Expected from notification of temperature higher than high threshold
        EXPECT_CALL(controller, Cool(true));
        EXPECT_CALL(controller, Heat(false));
        // Expected from notification of temperature higher than high threshold
        EXPECT_CALL(controller, Cool(true));
        EXPECT_CALL(controller, Heat(false));
        // Expected from notification of temperature lower than low threshold
        EXPECT_CALL(controller, Heat(true));
        EXPECT_CALL(controller, Cool(false));
        // Expected from notification of temperature higher than high threshold
        EXPECT_CALL(controller, Cool(true));
        EXPECT_CALL(controller, Heat(false));
        // Expected from notification of temperature lower than low threshold
        EXPECT_CALL(controller, Heat(true));
        EXPECT_CALL(controller, Cool(false));
        // Expected from notification of temperature lower than low threshold
        EXPECT_CALL(controller, Heat(true));
        EXPECT_CALL(controller, Cool(false));
        // Expected from notification of temperature lower than low threshold
        EXPECT_CALL(controller, Heat(true));
        EXPECT_CALL(controller, Cool(false));
        // Expected from notification of temperature higher than high threshold
        EXPECT_CALL(controller, Cool(true));
        EXPECT_CALL(controller, Heat(false));
    }

    Thermostat stat(meter, controller); 

    // Simulate thermometer calling the registered callback function indicating temperature thresholds breached.
    bool isHigh = true;
    stat.ThermometerCallback(isHigh);
    stat.ThermometerCallback(isHigh);
    isHigh = false;
    stat.ThermometerCallback(isHigh);
    isHigh = true;
    stat.ThermometerCallback(isHigh);
    isHigh = false;
    stat.ThermometerCallback(isHigh);
    stat.ThermometerCallback(isHigh);
    stat.ThermometerCallback(isHigh);
    isHigh = true;
    stat.ThermometerCallback(isHigh);
}

