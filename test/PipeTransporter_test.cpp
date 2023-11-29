#include <gtest/gtest.h>
#include "PipeTransporter.h"
#include "MockTopic.h"
#include "MockSerialiser.h"
#include "MockPipeTransporterImpl.h"

TEST(PipeTransporterTest, send) 
{
    std::string const pipename {"pubsub_testPipe"};
    auto serialiser = std::make_shared<MockSerialiser>();
    auto impl = std::make_shared<MockPipeTransporterImpl>();
    pubsub::PipeTransporter transporter { pipename, serialiser, impl };

    EXPECT_CALL(*impl, send_bytes)
        .Times(testing::Exactly(1));

    auto topic = std::make_shared<MockTopic>();
    transporter.send(topic);
}