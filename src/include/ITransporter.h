#pragma once

#include <memory>

namespace pubsub
{
    class ITopic;

    class ITransporter
    {
    public:
        virtual void send(std::shared_ptr<ITopic> const topic) = 0;
    };
}
