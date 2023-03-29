#include "gmock/gmock.h"

class MockTemperatureController: public TemperatureController {
public:
    MOCK_METHOD(void, Heat, (bool on), (override));
    MOCK_METHOD(void, Cool, (bool on), (override));
};
