#pragma once

#include <gmock/gmock.h> 
#include "pubsub.hpp"
#include "ITopic.h"
#include "ISerialiser.h"

class MockTopic : public pubsub::ITopic 
{
 public:
    MOCK_METHOD(void, serialise, (pubsub::ISerialiser const&), (override));
    PUBSUB_TOPIC("mock topic")
};