#include <string_view>
#include <algorithm>

#include "BasicDeserialiser.h"
#include "ITopic.h"
#include "ITopicReconstructor.h"
#include "Crc.h"

using namespace pubsub;

std::string const BasicDeserialiser::extract_string()
{
    auto size = extract<uint16_t>();
    std::string const result { mIter, mIter + size };
    mIter += size;
    return result;
}

void BasicDeserialiser::register_topic(std::string const& topicName, 
                                        ITopicReconstructor const& handler)
{
    mTopicReconstructors.insert({topicName, &handler});
}

std::shared_ptr<ITopic> const BasicDeserialiser::deserialise(Buffer const& buffer)
{
    mBuffer = &buffer;
    mIter = buffer.cget().begin();

    if(extract<uint16_t>() != MESSAGE_PREFIX)
        return nullptr;

    uint16_t const messageSize { extract<uint16_t>() };
    auto sizeIter = mIter;

    // Validate CRC.
    uint8_t const expectedCrc { extract<uint8_t>() };
    int64_t crcOffset{ mIter - buffer.cget().begin() };
    auto pBuffer = reinterpret_cast<uint8_t const*>(buffer.cget().data() + crcOffset);
    int64_t crcDataSize{ messageSize - crcOffset };
    uint8_t const actualCrc { Crc::get_instance().generate(pBuffer, crcDataSize)};

    if(actualCrc != expectedCrc)
        return nullptr;
    
    std::string const topicName { extract_string() };

    if(mTopicReconstructors.find(topicName) == mTopicReconstructors.end())
        return nullptr;

    return mTopicReconstructors[topicName]->deserialise_attributes(*this);
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
    value = extract<uint64_t>();
}