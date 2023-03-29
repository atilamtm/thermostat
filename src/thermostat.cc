#include "thermostat.h"

Thermostat::Thermostat(Thermometer &meter, TemperatureController &controller): 
        thermometer(meter), 
        tempController(controller), 
        highTemperatureThreshold(40), 
        lowTemperatureThreshold(10) {
    // Set thresholds before registering the callback to ensure no spurious callback is triggered.
    this->thermometer.SetTemperatureThresholds(this->highTemperatureThreshold, this->lowTemperatureThreshold);
    this->thermometer.RegisterCallback(std::bind_front(&Thermostat::ThermometerCallback, this));

    // First check of temperature needs to be manual, as the callback wasn't registered when the thresholds were set.    
    this->CheckTemperatureAndActManually();

    this->isOn = true;
}

void Thermostat::ThermometerCallback(bool isHigh) {
    // Only take action if the thermostat is enabled
    // TODO: Check if thermometer allows callback to be de-registered
    if (this->isOn) {
        if (isHigh) {
            this->tempController.Cool(true);
            this->tempController.Heat(false);
        }
        else {
            this->tempController.Heat(true);
            this->tempController.Cool(false);
        }
    }
}

bool Thermostat::SetTemperatureThresholds(int high, int low) {
    bool ret = false;
    // Only take action if the thresholds are valid
    if (high > low) {
        this->thermometer.SetTemperatureThresholds(high,low);
        ret = true;
    }
    return ret;
}

void Thermostat::EnableThermostat(bool on) {
    this->isOn = on;
    if (on) {
        this->CheckTemperatureAndActManually();
    }
}

// TODO: Current design is sub-optimal, as when the temperature in the room is surpasses either threshold, the temperature controller is invoked and stays active until a new temperature alarm is triggered, causing then the reverse operation to be triggered from the temperature controller, in an unending cycle.
void Thermostat::CheckTemperatureAndActManually() {
    int temp = this->thermometer.GetTemperature();
    if (temp < this->lowTemperatureThreshold) {
        this->tempController.Heat(true);
        this->tempController.Cool(false);
    }
    else if (temp > this->highTemperatureThreshold) {
        this->tempController.Cool(true);
        this->tempController.Heat(false);
    }
    else {
        this->tempController.Heat(false);
        this->tempController.Cool(false);
    }
}
