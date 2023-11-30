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
    std::string const getMessage() 
    {
        return message;
    }

    void process_attributes(pubsub::IMessageProcessor& messageProcessor) override
    {
        messageProcessor.attribute(message);
    }
    
    class TopicReconstructor : public pubsub::ITopicReconstructor
    {
    public:
        std::shared_ptr<ITopic> deserialise_attributes(IDeserialiser& deserialiser)
        {
            auto topic = std::make_shared<ExampleTopic>();
            topic->process_attributes(deserialiser);
            return topic;
        }
    };

    PUBSUB_TOPIC("example topic")
};
