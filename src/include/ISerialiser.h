#pragma once

#include <memory>
#include <vector>

namespace pubsub
{
    class ITopic;

    class ISerialiser
    {
    public:
        virtual void serialise_topic(std::shared_ptr<ITopic> const topic,
                                        std::vector<char>& output_buffer) = 0;
    };
}