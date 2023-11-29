#pragma once 

#include <map>
#include <string>
#include <vector>
#include <mutex>

#include "pubsub.hpp"
#include "IDeliverer.h"

namespace pubsub
{
    class Deliverer : IDeliverer
    {
    private:
        std::map<std::string, std::vector<SubscriberCallback>> mSubscriberMap {};
        std::mutex mMutex{};

        bool has_subscribers(std::string const& topicName);
    public:
        void register_subscriber(std::string const& topicName,
                                    SubscriberCallback callback);
        void on_data(std::string const& topicName,
                        std::shared_ptr<ITopic> const topic);
    };
}