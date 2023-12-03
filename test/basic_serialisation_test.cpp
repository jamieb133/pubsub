
#include <gtest/gtest.h>
#include "basic_serialisation.h"
#include "ITopic.h"
#include "IDeserialiser.h"
#include "pubsub_macros.h"

class TestTopic : public pubsub::ITopic
{
public:
    std::string message;
    uint8_t uint8;
    uint16_t uint16;
    uint32_t uint32;
    TestTopic() = default;

    bool const operator==(TestTopic const& other) const
    {
        bool result {true};
        result &= (other.message == message);
        result &= (other.uint8 == uint8);
        result &= (other.uint16 == uint16);
        result &= (other.uint32 == uint32);
        return result;
    }

    void process_attributes(pubsub::IMessageProcessor& messageProcessor) override
    {
        messageProcessor.attribute(message);
        messageProcessor.attribute(uint8);
        messageProcessor.attribute(uint16);
        messageProcessor.attribute(uint32);
    }

    PUBSUB_TOPIC(TestTopic)
};

static std::string const string_size_to_bytes(std::string const& val)
{
    uint16_t size { static_cast<uint16_t>(val.size()) };
    std::array<char,2> bytes {
        static_cast<char>((size >> 0) & 0xff),
        static_cast<char>((size >> 8) & 0xff)
    };
    return std::string { bytes.begin(), bytes.end() };
}

TEST(basic_serialisation, serialise_and_deserialise)
{
    auto inputTopic = std::make_shared<TestTopic>();
    inputTopic->message = "test message";
    inputTopic->uint8 = 100U;
    inputTopic->uint16 = 4096U;
    inputTopic->uint32 = 123456789;

    pubsub::basic_serialisation::BasicSerialiser serialiser{};
    pubsub::basic_serialisation::BasicDeserialiser deserialiser{};
    deserialiser.register_topic(inputTopic->get_name(), TestTopic::_get_reconstructor());
    std::array<char,pubsub::MAXIMUM_BUFFER_SIZE> buffer{};
    size_t size { serialiser.serialise(inputTopic, buffer) };
    
    pubsub::Buffer buf {};
    buf.get() = buffer;
    buf.set_size(size);    

    auto outputTopic = deserialiser.deserialise(buf);
    ASSERT_NE(outputTopic, nullptr);
    
    auto unwrappedTopic = std::dynamic_pointer_cast<TestTopic>(outputTopic);
    ASSERT_NE(unwrappedTopic, nullptr);
    EXPECT_EQ(*inputTopic, *unwrappedTopic);
} 

