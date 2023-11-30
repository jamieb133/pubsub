#pragma once

#include "IMessageProcessor.h"

namespace pubsub
{
    class ITopic 
    {
    public:
        virtual void process_attributes(IMessageProcessor& messageProcessor) = 0;
        virtual std::string const& get_name() = 0;
    };
}