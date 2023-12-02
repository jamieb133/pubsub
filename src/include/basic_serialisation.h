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

            template<typename T>
            void populate(typename std::enable_if<std::is_unsigned<T>::value, T>::type const val)
            {
                for(int i {0}; i < sizeof(T); i++)
                {
                    size_t rightShift { static_cast<size_t>(i * 8) };
                    *mIter++ = static_cast<char>((val >> rightShift) & 0xff);
                }
            }

        public:
            size_t serialise(std::shared_ptr<ITopic> const topic,
                                std::array<char,MAXIMUM_BUFFER_SIZE>& buffer);

            void attribute(std::string& value);
            void attribute(uint8_t& value);
            void attribute(uint16_t& value);
            void attribute(uint32_t& value);
            void attribute(uint64_t& value);
        };

        class BasicDeserialiser : public IDeserialiser
        {
            std::map<std::string,ITopicReconstructor const*> mTopicReconstructors{};
            std::vector<char> const* mBuffer{};
            std::vector<char>::const_iterator mIter{};

            bool const find(std::string const& val);

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

            std::string const extract_string()
            {
                auto size = extract<uint16_t>();
                std::string const result { mIter, mIter + size };
                mIter += size;
                return result;
            }

        public:
            std::shared_ptr<ITopic> const deserialise(std::vector<char> const& buffer);
            
            void register_topic(std::string const& topicName, 
                                ITopicReconstructor const& handler);

            void attribute(std::string& value);
            void attribute(uint8_t& value);
            void attribute(uint16_t& value);
            void attribute(uint32_t& value);
            void attribute(uint64_t& value);
        };
    }
}
