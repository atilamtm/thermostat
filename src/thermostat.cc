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
