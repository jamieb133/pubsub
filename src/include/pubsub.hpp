#pragma once

#include <functional>
#include <memory>

namespace pubsub
{
    class ITopic;
    
    using SubscriberCallback = std::function<void(std::shared_ptr<ITopic>)>;
}
