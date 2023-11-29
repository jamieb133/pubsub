#pragma once

#include <functional>
#include <memory>

#define PUBSUB_TOPIC(name)  static std::string const& _get_name() { static std::string const tName {name}; return tName; } \
                            std::string const& get_name() override { return _get_name(); }

namespace pubsub
{
    class ITopic;

    using SubscriberCallback = std::function<void(std::shared_ptr<ITopic>)>;
}
