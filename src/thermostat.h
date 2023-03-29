#ifndef _THERMOSTAT_H_
#define _THERMOSTAT_H_

#include <functional>
#include <gtest/gtest_prod.h>
#include "thermometer.h"
#include "temperature_controller.h"

/**
 * @brief Thermostat class receives temperature updates from the thermometer, and controls the temperature controller
 *        in order to adjust the temperature of the room.
 */
class Thermostat {
private:

    //TODO: Not a good practice to add test classes as friend. Design another way to allow test cases to simulate thermometer callbacks.
    FRIEND_TEST(ThermostatUnit, HeatWhenLowTemp);
    FRIEND_TEST(ThermostatUnit, CoolWhenHighTemp);
    FRIEND_TEST(ThermostatUnit, NoActionWhenThermostatDisabled);
    FRIEND_TEST(ThermostatUnit, ReenableThermostat);
    FRIEND_TEST(ThermostatUnit, HeatAndCoolSequence);

    Thermometer &thermometer; // Stores a reference to the thermometer that provides the room temperature info.
    TemperatureController &tempController; // Stores a reference to the temperature controller, that allows the 
                                          // room to be heated or cooled.
    int highTemperatureThreshold; // Stores the maximum desired temperature within the room.
    int lowTemperatureThreshold;  // Stores the minimum desired temperature within the room.
    bool isOn; // Store whether the thermostat should be on and controlling the room temperature. 

    // Callback used to receive notification from the thermometer class when the temperature thresholds are breached.
    void ThermometerCallback(bool isHigh);

    // Assistant function to facilitate code reuse. Checks the temperature from the thermometer and activates the 
    // temperature controller if the temperature is outside the high-low threshold boundary conditions.
    void CheckTemperatureAndActManually();
public:
    /**
     * @brief Creates a Thermostat object with references to a thermometer and a temperature controller.
              This constructor attributes default values for the temperature thresholds and enables the 
              thermostat.
     * @param therm the Thermometer to use when reading or receiving information on the temperature of the room.
     * @param tempCon the TemperatureController to use when heating or cooling a room.
     */
    Thermostat(Thermometer &therm, TemperatureController &tempCon); 

    /**
     * @brief Configures a new threshold for the maximum and minimum desired temperatures in the room.
     * @param high The new high temperature threshold. 
     * @param low  The new low temperature threshold.
     * @ret   True if the thresholds are valid (low < high), false otherwise.
     */
    bool SetTemperatureThresholds(int high, int low);

    /**
     * @brief Controls whether the thermostat is enabled and controlling the room temperature.
     * @param on True - enables the thermostat, False - disables the thermostat.
     */
    void EnableThermostat(bool on);
};

#endif //_THERMOSTAT_H_
