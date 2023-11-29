#pragma once

#include <string>

#include "pubsub.hpp"

namespace pubsub
{
    class IDeliverer
    {
    public:
        virtual void register_subscriber(std::string const& topicName,
                                    SubscriberCallback callback) = 0;
        virtual void on_data(std::string const& topicName,
                                std::shared_ptr<ITopic> const topic) = 0;
    };
}
