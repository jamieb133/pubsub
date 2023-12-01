#pragma once

#include <string>

namespace pubsub
{
    class IMessageProcessor
    {
    public:
        // Supported serialisation types.
        virtual void attribute(std::string& value) = 0;
        virtual void attribute(uint8_t& value) = 0;
    };
}
