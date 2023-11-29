#pragma once

#include <memory>

namespace pubsub
{
    class ITopic;
    class ITransporter;

    class IDistributor
    {
    public:
        virtual void send(std::shared_ptr<ITopic> topic) = 0;
        virtual void add_transporter(std::shared_ptr<ITransporter> transporter) = 0;
    };
}
