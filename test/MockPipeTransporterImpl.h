#pragma once

#include <gmock/gmock.h> 
#include "IPipeTransporterImpl.h"

class MockPipeTransporterImpl :public pubsub::IPipeTransporterImpl
{
public:
    using Buffer = std::array<char,pubsub::MAXIMUM_BUFFER_SIZE> const&;

    MOCK_METHOD(void, send_bytes, (Buffer,size_t), (override));
};
