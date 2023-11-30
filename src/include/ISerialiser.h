#pragma once

#include <memory>
#include <vector>

#include "pubsub_macros.h"
#include "IMessageProcessor.h"

namespace pubsub
{
    class ITopic;

    class ISerialiser : public IMessageProcessor
    {
    public:
        virtual size_t serialise(std::shared_ptr<ITopic> const topic,
                                    std::array<char,MAXIMUM_BUFFER_SIZE>& buffer) = 0;
    };
}