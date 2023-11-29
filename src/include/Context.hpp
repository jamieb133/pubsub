#pragma once

#include "pubsub.hpp"
#include "Context.h"
#include "IDeliverer.h"

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
        deliverer->register_subscriber(TopicType::getName(), wrappedCallback);
    }
}