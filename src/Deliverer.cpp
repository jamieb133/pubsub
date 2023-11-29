#include "pubsub.hpp"
#include "Deliverer.h"
#include "ITopic.h"

using namespace pubsub;

bool Deliverer::has_subscribers(std::string const& topicName)
{
    return mSubscriberMap.find(topicName) != mSubscriberMap.end();
}

void Deliverer::register_subscriber(std::string const& topicName,
                                        SubscriberCallback callback)
{
    std::lock_guard<std::mutex> lock { mMutex };

    if (!has_subscribers(topicName))
    {
        // Initialise the callback list.
        mSubscriberMap.insert({topicName, std::vector<SubscriberCallback>{}});
    }

    mSubscriberMap[topicName].push_back(callback);
}

void Deliverer::on_data(std::shared_ptr<ITopic> const topic)
{
    std::lock_guard<std::mutex> lock { mMutex };
    std::string const& topicName { topic->get_name() };

    if (!has_subscribers(topicName))
    {
        return;
    }
    
    for(auto const& callback : mSubscriberMap[topicName])
    {
        callback(topic);
    }
}
