#ifdef _THERMOMETER_H_
#define _THERMOMETER_H_

/**
 * @brief Thermometer class that represents a thermometer component. Just as a basis to build a thermostat.
 *        Should be discarded when an actual thermometer implementation is available. 
 */
class Thermometer {
public:
    /* Create a new Thermometer object.
     * @param callback A function that will be called whenever the temperature exceeds either threshold.
     */
    Thermometer(void (*callback)(bool isHigh)) = 0;

    /**
     * @brief Reads the currentn temperature and returns the value read.
     * @return The value of the current temperature. 
     */
    int GetTemperature() = 0;
    /**
     * @brief Configures new values for high and low temperature thresholds. If the current temperature
     *        is exceeding either threshold, the temperatureThresholdReachedCallback is immediately triggered.
     * @param high The new high temperature threshold.
     * @param low the new low temperature threshold.
     */
    void SetTemperatureThresholds(int high, int low) = 0;
}

#endif //_THERMOMETER_H_
