#pragma once

#include <gmock/gmock.h> 
#include "ITopic.h"
#include "ISerialiser.h"

class MockTopic : public pubsub::ITopic 
{
 public:
    MOCK_METHOD(void, serialise, (pubsub::ISerialiser const&), (override));
    static std::string const& getName() { static std::string const name {"mock topic"}; return name; }
};