#ifndef _THERMOMETER_H_
#define _THERMOMETER_H_

#include <functional>

/**
 * @brief Thermometer class that represents a thermometer component. Just as a basis to build a thermostat.
 *        Should be discarded when an actual thermometer implementation is available. 
 */
class Thermometer {
public:

    virtual ~Thermometer() {}

    /**
     * @brief Reads the currentn temperature and returns the value read.
     * @return The value of the current temperature. 
     */
    virtual int GetTemperature() const = 0;

    /**
     * @brief Configures new values for high and low temperature thresholds. If the current temperature
     *        is exceeding either threshold, the temperatureThresholdReachedCallback is immediately triggered.
     * @param high The new high temperature threshold. If greater than the max measurable temperature, the max is used.
     * @param low the new low temperature threshold. If lower than the min measurable temperature, the min is used.
     * @ret   True if the setting is accepted (low < high), false otherwise
     */
    virtual bool SetTemperatureThresholds(int high, int low) = 0;

    /**
     * @brief Configures a callback to be called when the temperature raises above the high threshold or below the low
     *        threshold.
     * @param callback A function that will be called whenever the temperature exceeds either threshold.
     */
    virtual void RegisterCallback(std::function<void (bool)> callback) = 0;
};

#endif //_THERMOMETER_H_
