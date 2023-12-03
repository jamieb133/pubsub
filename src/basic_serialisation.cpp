#include <string_view>
#include <algorithm>

#include "basic_serialisation.h"
#include "ITopic.h"
#include "ITopicReconstructor.h"

using namespace pubsub;
using namespace pubsub::basic_serialisation;

static std::array<char,2> const string_size_to_bytes(std::string const& val)
{
    uint16_t size { static_cast<uint16_t>(val.size()) };
    return std::array<char,2> {
        static_cast<char>((size >> 0) & 0xff),
        static_cast<char>((size >> 8) & 0xff)
    };
}

void BasicSerialiser::populate_string(std::string const& val)
{
    // Add length of string.
    std::array<char,2> len { string_size_to_bytes(val) };
    mIter = std::copy(len.begin(), len.end(), mIter);

    // Copy string content.
    mIter = std::copy(val.begin(), val.end(), mIter);
}

std::string const BasicDeserialiser::extract_string()
{
    auto size = extract<uint16_t>();
    std::string const result { mIter, mIter + size };
    mIter += size;
    return result;
}

bool const BasicDeserialiser::find(std::string const& val)
{
    auto buffer = mBuffer->cget();
    std::string const current { buffer.data(), buffer.size() };
    size_t index { current.find(val) };
    if(index == std::string::npos)
        return false;
    mIter += index + val.size();
    return true;
}

size_t BasicSerialiser::serialise(std::shared_ptr<ITopic> const topic,
                                    std::array<char,MAXIMUM_BUFFER_SIZE>& buffer)
{
    mBuffer = &buffer;

    // Add message preamble.
    mIter = std::copy(MESSAGE_PREFIX.begin(), MESSAGE_PREFIX.end(), buffer.begin());

    // Add topic name size.
    std::string const& topicName { topic->get_name() };
    populate_string(topicName);

    // Add attributes.
    topic->process_attributes(*this);

    return mIter - buffer.begin();
}

std::shared_ptr<ITopic> const BasicDeserialiser::deserialise(Buffer const& buffer)
{
    mBuffer = &buffer;
    mIter = buffer.cget().begin();

    if(!find(MESSAGE_PREFIX))
        return nullptr;

    std::string const topicName = extract_string();

    if(mTopicReconstructors.find(topicName) == mTopicReconstructors.end())
        return nullptr;

    return mTopicReconstructors[topicName]->deserialise_attributes(*this);
}

void BasicSerialiser::attribute(std::string& value)
{
    populate_string(value);
}

void BasicSerialiser::attribute(uint8_t& value)
{
    populate<uint8_t>(value);
}

void BasicDeserialiser::attribute(std::string& value)
{
    value = extract_string();
}

void BasicDeserialiser::attribute(uint8_t& value)
{
    value = extract<uint8_t>();
}

void BasicDeserialiser::attribute(uint16_t& value)
{
    value = extract<uint16_t>();
}

void BasicDeserialiser::attribute(uint32_t& value)
{
    value = extract<uint32_t>();
}

void BasicDeserialiser::attribute(uint64_t& value)
{
    value = extract<uint32_t>();
}

void BasicSerialiser::attribute(uint16_t& value)
{
    populate<uint16_t>(value);
}

void BasicSerialiser::attribute(uint32_t& value)
{
    populate<uint32_t>(value);
}

void BasicSerialiser::attribute(uint64_t& value)
{
    populate<uint64_t>(value);
}

void BasicDeserialiser::register_topic(std::string const& topicName, 
                                        ITopicReconstructor const& handler)
{
    mTopicReconstructors.insert({topicName, &handler});
}
