#pragma once

#include <gmock/gmock.h> 
#include "IDistributor.h"

class MockDistributor :public pubsub::IDistributor
{
public:
    MOCK_METHOD(void, send, (std::shared_ptr<ITopic>), (override));
    MOCK_METHOD(void, add_transporter, (std::shared_ptr<ITransporter>), (override));
};
