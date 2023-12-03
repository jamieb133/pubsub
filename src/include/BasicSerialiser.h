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
    class BasicSerialiser : public ISerialiser
    {
    private:
        std::array<char,MAXIMUM_BUFFER_SIZE>* mBuffer{};
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

        void populate_string(std::string const& val);

    public:
        size_t serialise(std::shared_ptr<ITopic> const topic,
                            std::array<char,MAXIMUM_BUFFER_SIZE>& buffer);

        void attribute(std::string& value);
        void attribute(uint8_t& value);
        void attribute(uint16_t& value);
        void attribute(uint32_t& value);
        void attribute(uint64_t& value);
    };
}
