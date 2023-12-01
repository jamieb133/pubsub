#pragma once

#include <gmock/gmock.h> 
#include "IClient.h"

class MockClient :public pubsub::IClient
{
public:
    using Buffer = std::array<char,pubsub::MAXIMUM_BUFFER_SIZE> const&;

    MOCK_METHOD(void, send_bytes, (Buffer,size_t), (override));
};
