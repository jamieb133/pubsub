#include <string>

#include "pubsub.hpp"
#include "ITopic.h"
#include "ITopicReconstructor.h"
#include "ISerialiser.h"
#include "IDeserialiser.h"

class ExampleTopic;

class ExampleTopic : public pubsub::ITopic
{
public:
    std::string message {};
    uint8_t uint8{};
    uint16_t uint16{};
    uint32_t uint32{};
    uint32_t uint64{};

    ExampleTopic() = default;

    std::string const getMessage() 
    {
        return message;
    }

    void process_attributes(pubsub::IMessageProcessor& messageProcessor) override
    {
        messageProcessor.attribute(message);
        messageProcessor.attribute(uint8);
        messageProcessor.attribute(uint16);
        messageProcessor.attribute(uint32);
    }
    
    PUBSUB_TOPIC(ExampleTopic)
};
