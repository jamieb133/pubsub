#pragma once

#include <string>

namespace pubsub
{
    class ITopicReconstructor;

    class IServer
    {
    public:
        virtual void register_topic(std::string const& topicName,
                                    ITopicReconstructor const& topicReconstructor) = 0;
    };
}
