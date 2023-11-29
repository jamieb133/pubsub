#pragma once

#include <gmock/gmock.h> 
#include "ITransporter.h"

class MockTransporter :public pubsub::ITransporter
{
public:
    MOCK_METHOD(void, send, (std::shared_ptr<ITopic> const), (override));
};
