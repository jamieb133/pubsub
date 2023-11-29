#pragma once

#include <gmock/gmock.h> 
#include "pubsub.hpp"
#include "IDeliverer.h"

class MockDeliverer :public pubsub::IDeliverer
{
public:
    MOCK_METHOD(void, register_subscriber, (std::string const&,pubsub::SubscriberCallback), (override));
    MOCK_METHOD(void, on_data, (std::shared_ptr<pubsub::ITopic> const), (override));
};
