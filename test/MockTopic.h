#pragma once

#include <gmock/gmock.h> 
#include "pubsub.hpp"
#include "ITopic.h"
#include "ISerialiser.h"
#include "ITopicReconstructor.h"

class MockTopic : public pubsub::ITopic 
{
 public:
    MOCK_METHOD(void, process_attributes, (pubsub::IMessageProcessor&), (override));
    PUBSUB_TOPIC(MockTopic)
};