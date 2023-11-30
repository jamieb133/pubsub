#pragma once

#include <memory>
#include <vector>

#include "pubsub_macros.h"
#include "IMessageProcessor.h"

namespace pubsub
{
    class ITopic;
    class ITopicReconstructor;

    class IDeserialiser : public IMessageProcessor
    {
    public:
        virtual std::shared_ptr<ITopic> const deserialise(std::vector<char> const& buffer) = 0;
        virtual void register_topic(std::string const& topicName,
                                    ITopicReconstructor const& topicReconstructor) = 0;
    };
}