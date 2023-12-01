#pragma once

#include "pubsub.hpp"
#include "Context.h"
#include "IDeliverer.h"
#include "IReceiver.h"

using namespace pubsub;

template <class TopicType>
void Context::subscribe(std::function<void(std::shared_ptr<TopicType>)> callback)
{
    SubscriberCallback wrappedCallback 
    {
        [callback](std::shared_ptr<ITopic> const topic)
        {
            auto const unwrappedTopic = std::dynamic_pointer_cast<TopicType>(topic);
            if(unwrappedTopic)
            {
                callback(unwrappedTopic);
            }
        }
    };

    for(std::shared_ptr<IDeliverer>& deliverer : mDeliverers)
    {
        deliverer->register_subscriber(TopicType::_get_name(), wrappedCallback);
    }

    for(std::shared_ptr<IReceiver>& receiver : mReceivers)
    {
        receiver->register_topic(TopicType::_get_name(), TopicType::_get_reconstructor());
    }
}