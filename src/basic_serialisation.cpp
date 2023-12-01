#include <string_view>
#include <algorithm>

#include "basic_serialisation.h"
#include "ITopic.h"
#include "ITopicReconstructor.h"

using namespace pubsub;
using namespace pubsub::basic_serialisation;

static uint16_t const bytes_to_unsigned_short(char const b1,
                                        char const b2)
{
    auto lsb = static_cast<uint16_t>((b1 << 0) & 0x00ff);
    auto msb = static_cast<uint16_t>((b2 << 8) & 0xff00);
    return msb | lsb;
}

static std::array<char,2> unsigned_short_to_bytes(uint16_t const val)
{
    return std::array<char,2> {
        static_cast<char>((val >> 0) & 0xff),
        static_cast<char>((val >> 8) & 0xff)
    };
}

static std::array<char,2> const string_size_to_bytes(std::string const& val)
{
    uint16_t size { static_cast<uint16_t>(val.size()) };
    return unsigned_short_to_bytes(size);
}

size_t BasicSerialiser::serialise(std::shared_ptr<ITopic> const topic,
                                    std::array<char,MAXIMUM_BUFFER_SIZE>& buffer)
{
    mBuffer = &buffer;

    // Add message preamble.
    mIter = std::copy(MESSAGE_PREFIX.begin(), MESSAGE_PREFIX.end(), buffer.begin());

    // Add topic name size.
    std::string const& topicName { topic->get_name() };
    std::array<char,2> const sizeBytes { string_size_to_bytes(topicName) };
    mIter = std::copy(sizeBytes.begin(), sizeBytes.end(), mIter);

    // Add topic name.
    mIter = std::copy(topicName.begin(), topicName.end(), mIter);

    // Add attributes.
    topic->process_attributes(*this);

    return mIter - buffer.begin();
}

std::shared_ptr<ITopic> const BasicDeserialiser::deserialise(std::vector<char> const& buffer)
{
    mBuffer = &buffer;
    mIter = buffer.begin();

    // TODO: zero copy here...
    std::string const message { buffer.data(), buffer.data() + buffer.size() };

    size_t index { message.find(MESSAGE_PREFIX) };
    if(index == std::string::npos)
        return nullptr;

    // Get topic name size.
    index += MESSAGE_PREFIX.size();
    uint16_t nameSize { bytes_to_unsigned_short(buffer[index], buffer[index+1]) };

    index +=2;
    auto start = buffer.data() + index;
    auto end = start + nameSize;
    std::string const topicName { start, end };

    if(mTopicReconstructors.find(topicName) == mTopicReconstructors.end())
        return nullptr;

    index += topicName.size();
    mIter += index;

    return mTopicReconstructors[topicName]->deserialise_attributes(*this);
}

void BasicSerialiser::attribute(std::string& value)
{
    // Add length of string.
    std::array<char,2> len { string_size_to_bytes(value) };
    mIter = std::copy(len.begin(), len.end(), mIter);

    // Copy string content.
    mIter = std::copy(value.begin(), value.end(), mIter);
}

void BasicDeserialiser::attribute(std::string& value)
{
    uint16_t size { bytes_to_unsigned_short(*mIter++, *mIter++) };
    if(size > mBuffer->size())
    {
        return;
    }
    value = std::string { mIter, mIter + size };
    mIter += size;
}

void BasicSerialiser::attribute(uint8_t& value)
{
    *mIter = value;
    mIter++;
}

void BasicDeserialiser::attribute(uint8_t& value)
{
    value = *mIter;
    mIter++;
}

void BasicSerialiser::attribute(uint16_t& value)
{
    auto bytes = unsigned_short_to_bytes(value);
    *mIter++ = bytes[0];
    *mIter++ = bytes[1];
}

void BasicDeserialiser::attribute(uint16_t& value)
{
    value = bytes_to_unsigned_short(*mIter++, *mIter++);
}

void BasicDeserialiser::register_topic(std::string const& topicName, 
                                        ITopicReconstructor const& handler)
{
    mTopicReconstructors.insert({topicName, &handler});
}
