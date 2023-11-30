#pragma once

#include <memory>

namespace pubsub
{
    class ISerialiser;
    class IDeserialiser;
    class ITopic;

    class ITopicReconstructor
    {
    public:
        virtual std::shared_ptr<ITopic> const deserialise_attributes(IDeserialiser& deserialiser) const = 0;
    };
}