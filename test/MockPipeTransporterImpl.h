#pragma once

#include <gmock/gmock.h> 
#include "IPipeTransporterImpl.h"

class MockPipeTransporterImpl :public pubsub::IPipeTransporterImpl
{
public:
    MOCK_METHOD(void, send_bytes, (std::vector<char> const&), (override));
};
