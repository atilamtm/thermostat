#include "gmock/gmock.h"

class MockThermometer: public Thermometer {
public:

    MOCK_METHOD(int, GetTemperature, (), (const, override));
    MOCK_METHOD(void, SetTemperatureThresholds, (int high, int low), (override));
};
