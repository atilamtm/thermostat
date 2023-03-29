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

TEST(ThermostatUnit, StartWithHightemp) {
    MockThermometer meter;
    MockTemperatureController controller;

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

TEST(ThermostatUnit, StartWithLowTemp) {
    MockThermometer meter;
    MockTemperatureController controller;

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

    EXPECT_CALL(meter, SetTemperatureThresholds(20,15))
        .WillOnce(Return(true)); 
    EXPECT_CALL(meter, SetTemperatureThresholds(10,5))
        .WillOnce(Return(true)); 
    EXPECT_CALL(meter, SetTemperatureThresholds(80,0))
        .WillOnce(Return(true)); 
    EXPECT_CALL(meter, SetTemperatureThresholds(40,10))
        .WillOnce(Return(true)); 

    Thermostat stat(meter, controller);
    stat.SetTemperatureThresholds(80,0);
    stat.SetTemperatureThresholds(10,5);
    stat.SetTemperatureThresholds(20,15);
}

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
    EXPECT_EQ(stat.SetTemperatureThresholds(-5,0), false);
    EXPECT_EQ(stat.SetTemperatureThresholds(10,10), false);
    EXPECT_EQ(stat.SetTemperatureThresholds(5,10), false);
    EXPECT_EQ(stat.SetTemperatureThresholds(20,80), false);
}

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
    bool isHigh = true;
    stat.ThermometerCallback(isHigh);
    stat.ThermometerCallback(isHigh);
    isHigh = false;
    stat.ThermometerCallback(isHigh);
    isHigh = true;
    stat.ThermometerCallback(isHigh);
}

TEST(ThermostatUnit, ReenableThermostat) {
    MockThermometer meter;
    MockTemperatureController controller;

    // Constructor
    EXPECT_CALL(meter, SetTemperatureThresholds(_,_))
        .Times(1)
        .WillOnce(Return(true)); 
    EXPECT_CALL(meter, RegisterCallback(_))
        .Times(1); 
    EXPECT_CALL(meter, GetTemperature())
        .Times(4)
        .WillOnce(Return(20))
        .WillOnce(Return(20))
        .WillOnce(Return(50))
        .WillOnce(Return(-10));

    EXPECT_CALL(controller, Heat(true))
        .Times(2); 
    EXPECT_CALL(controller, Heat(false))
        .Times(4); 
    EXPECT_CALL(controller, Cool(true))
        .Times(2); 
    EXPECT_CALL(controller, Cool(false))
        .Times(4); 

    Thermostat stat(meter, controller);

    stat.EnableThermostat(false);   
 
    // Simulate thermometer calling the registered callback function indicating temperature thresholds breached.
    bool isHigh = true;
    stat.ThermometerCallback(isHigh);
    isHigh = false;
    stat.ThermometerCallback(isHigh);

    stat.EnableThermostat(true);   
 
    isHigh = false;
    stat.ThermometerCallback(isHigh);
    isHigh = true;
    stat.ThermometerCallback(isHigh);

    stat.EnableThermostat(false);   
    stat.EnableThermostat(true);   

    stat.EnableThermostat(false);   
    stat.EnableThermostat(true);   
}

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

        EXPECT_CALL(controller, Heat(false));
        EXPECT_CALL(controller, Cool(false));
    
        EXPECT_CALL(controller, Cool(true));
        EXPECT_CALL(controller, Heat(false));
        EXPECT_CALL(controller, Cool(true));
        EXPECT_CALL(controller, Heat(false));
        EXPECT_CALL(controller, Heat(true));
        EXPECT_CALL(controller, Cool(false));
        EXPECT_CALL(controller, Cool(true));
        EXPECT_CALL(controller, Heat(false));
        EXPECT_CALL(controller, Heat(true));
        EXPECT_CALL(controller, Cool(false));
        EXPECT_CALL(controller, Heat(true));
        EXPECT_CALL(controller, Cool(false));
        EXPECT_CALL(controller, Heat(true));
        EXPECT_CALL(controller, Cool(false));
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

