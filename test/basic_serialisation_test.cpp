
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
    TestTopic() = default;

    void process_attributes(pubsub::IMessageProcessor& messageProcessor) override
    {
        messageProcessor.attribute(message);
        messageProcessor.attribute(uint8);
    }

    PUBSUB_TOPIC("test_topic", TestTopic)
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

TEST(basic_serialisation, serialise_topic) 
{
    return;
    pubsub::basic_serialisation::BasicSerialiser serialiser {};
    auto topic = std::make_shared<TestTopic>();
    topic->message = "test message";
    std::array<char,pubsub::MAXIMUM_BUFFER_SIZE> buffer{};

    size_t size { serialiser.serialise(topic, buffer) };

    uint16_t attributeSize { static_cast<uint16_t>(topic->message.size()) };

    auto nameSize = string_size_to_bytes(topic->get_name());
    auto attrSize = string_size_to_bytes(topic->message);

    std::string const expected { "pubsub--" + nameSize + topic->get_name() + attrSize + topic->message };
    std::string const actual { buffer.begin(), size };

    EXPECT_EQ(actual, expected);
}

TEST(basic_serialisation, deserialise_buffer) 
{
    return;
    pubsub::basic_serialisation::BasicDeserialiser deserialiser{};
    TestTopic expectedTopic { };
    expectedTopic.message = "test message";
    deserialiser.register_topic(expectedTopic.get_name(), TestTopic::_get_reconstructor());

    auto nameSize = string_size_to_bytes(expectedTopic.get_name());
    auto attrSize = string_size_to_bytes(expectedTopic.message);

    std::string const serialised { "pubsub--" + nameSize + expectedTopic.get_name() + attrSize + expectedTopic.message };
    std::vector<char> const buffer { serialised.begin(), serialised.end() };

    auto topic = deserialiser.deserialise(buffer);

    ASSERT_NE(topic, nullptr);

    auto unwrappedTopic {std::dynamic_pointer_cast<TestTopic>(topic)};

    ASSERT_NE(unwrappedTopic, nullptr);
    EXPECT_EQ(unwrappedTopic->message, expectedTopic.message);
}

TEST(basic_serialisation, serialise_and_deserialise)
{
    auto inputTopic = std::make_shared<TestTopic>();
    inputTopic->message = "test message";
    inputTopic->uint8 = 100U;

    pubsub::basic_serialisation::BasicSerialiser serialiser{};
    pubsub::basic_serialisation::BasicDeserialiser deserialiser{};
    deserialiser.register_topic(inputTopic->get_name(), TestTopic::_get_reconstructor());
    std::array<char,pubsub::MAXIMUM_BUFFER_SIZE> buffer{};
    size_t size { serialiser.serialise(inputTopic, buffer) };
    
    auto outputTopic = deserialiser.deserialise(std::vector<char>{buffer.data(), buffer.data() + size});
    ASSERT_NE(outputTopic, nullptr);
    
    auto unwrappedTopic = std::dynamic_pointer_cast<TestTopic>(outputTopic);
    ASSERT_NE(unwrappedTopic, nullptr);

    EXPECT_EQ(inputTopic->message, unwrappedTopic->message);
    EXPECT_EQ(inputTopic->uint8, unwrappedTopic->uint8);
} 

