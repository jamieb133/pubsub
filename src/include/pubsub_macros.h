#pragma once

#include <functional>
#include <memory>

#define PUBSUB_TOPIC(TopicType) \
    static std::string const& _get_name() { static std::string const tName {#TopicType}; return tName; } \
    std::string const& get_name() override { return _get_name(); } \
    static pubsub::ITopicReconstructor const& _get_reconstructor() \
    { \
        static TopicReconstructor instance {}; \
        return instance; \
    } \
    class TopicReconstructor : public pubsub::ITopicReconstructor \
    { \
    public: \
        std::shared_ptr<ITopic> const deserialise_attributes(pubsub::IDeserialiser& deserialiser) const \
        { \
            auto topic = std::make_shared<TopicType>(); \
            topic->process_attributes(deserialiser); \
            return topic; \
        } \
    };

namespace pubsub
{
    class ITopic;
    using SubscriberCallback = std::function<void(std::shared_ptr<ITopic>)>;

    constexpr int MAXIMUM_BUFFER_SIZE { 1024 };

}
