#pragma once

#include <gmock/gmock.h> 
#include <vector>
#include "ISerialiser.h"
#include "ITopic.h"

class MockSerialiser :public pubsub::ISerialiser
{
public:
    MOCK_METHOD(void, serialise_topic, (std::shared_ptr<pubsub::ITopic> const,
                                        std::vector<char>&), (override));
};
