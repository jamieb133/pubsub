#include <string>

#include "pubsub.hpp"
#include "ITopic.h"
#include "ITopicReconstructor.h"
#include "ISerialiser.h"
#include "IDeserialiser.h"

class ExampleTopic;

class ExampleTopic : public pubsub::ITopic
{
private:
    std::string message {};

public:
    ExampleTopic() = default;
    ExampleTopic(std::string const& msg) : message{msg}{}

    std::string const getMessage() 
    {
        return message;
    }

    void process_attributes(pubsub::IMessageProcessor& messageProcessor) override
    {
        messageProcessor.attribute(message);
    }
    
    PUBSUB_TOPIC("example topic", ExampleTopic)
};
