#include <string_view>
#include <algorithm>

#include "BasicSerialiser.h"
#include "ITopic.h"
#include "ITopicReconstructor.h"
#include "Crc.h"

using namespace pubsub;

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


size_t BasicSerialiser::serialise(std::shared_ptr<ITopic> const topic,
                                    std::array<char,MAXIMUM_BUFFER_SIZE>& buffer)
{
    mBuffer = &buffer;
    mIter = buffer.begin();

    // Add message preamble.
    populate<uint16_t>(MESSAGE_PREFIX);

    // Reserve 16 bits for size.
    auto sizeIter = mIter;
    mIter += 2;

    // Reserve 8 bits for CRC.
    auto crcIter = mIter;
    mIter++;

    // Add topic name.
    std::string const& topicName { topic->get_name() };
    populate_string(topicName);

    // Add attributes.
    topic->process_attributes(*this);

    // Add message size, mIter will now be at the end of the last attribute which marks
    // the end of the message. 
    auto eomIter = mIter;
    uint16_t messageSize {static_cast<uint16_t>(eomIter - buffer.begin())};
    mIter = sizeIter;
    populate<uint16_t>(messageSize);

    // Add crc.
    auto crcStartIter = crcIter + 1;
    int64_t crcOffset{crcStartIter - buffer.begin()};
    auto pBuffer = reinterpret_cast<uint8_t*>(buffer.data() + crcOffset);
    int64_t crcDataSize{ messageSize - crcOffset };
    uint8_t crc { Crc::get_instance().generate(pBuffer, messageSize - crcOffset) };
    populate<uint8_t>(crc);

    return messageSize;
}

void BasicSerialiser::attribute(std::string& value)
{
    populate_string(value);
}

void BasicSerialiser::attribute(uint8_t& value)
{
    populate<uint8_t>(value);
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
