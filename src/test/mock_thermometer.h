#include "gmock/gmock.h"

class MockThermometer: public Thermometer {
public:

    MOCK_METHOD(int, GetTemperature, (), (const, override));
    MOCK_METHOD(bool, SetTemperatureThresholds, (int high, int low), (override));
    MOCK_METHOD(void, RegisterCallback, (std::function<void (bool)> callback), (override));
};
