#pragma once

#include <memory>
#include <vector>

namespace pubsub
{
    class ITopic;

    class IDeserialiser
    {
    public:
        virtual std::shared_ptr<ITopic> const deserialise_bytes(std::vector<char> const& buffer) = 0;
    };
}