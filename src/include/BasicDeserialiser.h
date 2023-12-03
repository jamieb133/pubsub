#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <map>
#include <array>
#include <algorithm>
#include <type_traits>
#include <limits>
#include <iostream>

#include "IDeserialiser.h"
#include "IDeliverer.h"
#include "ITopicReconstructor.h"
#include "pubsub_macros.h"

namespace pubsub
{
    class BasicDeserialiser : public IDeserialiser
    {
        std::map<std::string,ITopicReconstructor const*> mTopicReconstructors{};
        Buffer const* mBuffer{};
        std::array<char, MAXIMUM_BUFFER_SIZE>::const_iterator mIter{};

        template <typename T>
        typename std::enable_if<std::is_unsigned<T>::value, T>::type extract()
        {
            T result{0};
            for(int i = 0; i < sizeof(T); i++)
            {
                auto current = static_cast<uint8_t>(*mIter++);
                size_t const leftShift { static_cast<size_t>(i * 8) }; 
                auto mask = 0xff << leftShift;
                result |= static_cast<T>((current << leftShift) & mask);
            }

            return result;
        }

        std::string const extract_string();

    public:
        std::shared_ptr<ITopic> const deserialise(Buffer const& buffer);
        
        void register_topic(std::string const& topicName, 
                            ITopicReconstructor const& handler);

        void attribute(std::string& value);
        void attribute(uint8_t& value);
        void attribute(uint16_t& value);
        void attribute(uint32_t& value);
        void attribute(uint64_t& value);
    };
}
