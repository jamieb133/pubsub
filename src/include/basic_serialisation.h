#pragma once

#include <string>
#include <string_view>
#include <map>
#include <array>
#include <algorithm>
#include <iostream>

#include "ISerialiser.h"
#include "IDeserialiser.h"
#include "IDeliverer.h"
#include "ITopicReconstructor.h"
#include "pubsub_macros.h"

namespace pubsub
{
    namespace basic_serialisation
    {
        std::string const MESSAGE_PREFIX { "pubsub--" };

        class BasicSerialiser : public ISerialiser
        {
        private:
            std::array<char,MAXIMUM_BUFFER_SIZE> const* mBuffer{};
            std::array<char,MAXIMUM_BUFFER_SIZE>::iterator mIter{};
            size_t mCurrentMessageSize{};
        public:
            size_t serialise(std::shared_ptr<ITopic> const topic,
                                std::array<char,MAXIMUM_BUFFER_SIZE>& buffer);

            void attribute(std::string& value);

        };

        class BasicDeserialiser : public IDeserialiser
        {
            std::map<std::string,ITopicReconstructor const*> mTopicReconstructors{};
            std::vector<char> const* mBuffer{};
            std::vector<char>::const_iterator mIter{};

            std::string_view const get_substring(std::string const& startDelim,
                                                    std::string const& endDelim);
        public:
            std::shared_ptr<ITopic> const deserialise(std::vector<char> const& buffer);
            
            void register_topic(std::string const& topicName, 
                                ITopicReconstructor const& handler);

            void attribute(std::string& value);
        };
    }
}
