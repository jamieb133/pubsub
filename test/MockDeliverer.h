#pragma once

#include <gmock/gmock.h> 
#include "IDeliverer.h"

class MockDeliverer :public pubsub::IDeliverer
{
public:
    MOCK_METHOD(void, register_subscriber, (std::string const&,SubscriberCallback), (override));
    MOCK_METHOD(void, on_data, (std::shared_ptr<ITopic> const), (override));
};
