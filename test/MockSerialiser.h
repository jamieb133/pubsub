#pragma once

#include <gmock/gmock.h> 
#include <array>
#include <memory>
#include "ISerialiser.h"
#include "ITopic.h"

class MockSerialiser :public pubsub::ISerialiser
{
public:
    using TopicPtr = std::shared_ptr<pubsub::ITopic> const;
    using Buffer = std::array<char,pubsub::MAXIMUM_BUFFER_SIZE>&;

    MOCK_METHOD(size_t, serialise, (TopicPtr,Buffer), (override));
    MOCK_METHOD(void, attribute, (std::string&), (override));
    MOCK_METHOD(void, attribute, (uint8_t&), (override));
};
