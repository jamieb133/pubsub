#include <string>

#include "pubsub.hpp"
#include "ITopic.h"

class ExampleTopic : public pubsub::ITopic
{
private:
    std::string message {};
public:
    std::string const getMessage() 
    {
        return message;
    }

    ExampleTopic(std::string const& messageIn) :
        message { messageIn }
    {}

    ExampleTopic(ExampleTopic const& other) :
        message { other.message }
    {}

    ExampleTopic(ExampleTopic&& other) :
        message { other.message }
    {}

    ExampleTopic& operator=(ExampleTopic const& other)
    {
        message = other.message;
        return *this;
    }

    ExampleTopic& operator=(ExampleTopic&& other)
    {
        message = other.message;
        return *this;
    }

    void serialise(pubsub::ISerialiser const& serialiser) override
    {

    }

    PUBSUB_TOPIC("example topic");
};
