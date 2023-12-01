#include <gtest/gtest.h>
#include "IndirectTransporter.h"
#include "MockTopic.h"
#include "MockSerialiser.h"
#include "MockClient.h"

TEST(IndirectTransporterTest, send) 
{
    auto serialiser = std::make_shared<MockSerialiser>();
    auto client = std::make_shared<MockClient>();
    pubsub::IndirectTransporter transporter { serialiser, client };

    EXPECT_CALL(*serialiser, serialise)
        .Times(testing::Exactly(1));
    EXPECT_CALL(*client, send_bytes)
        .Times(testing::Exactly(1));

    auto topic = std::make_shared<MockTopic>();
    transporter.send(topic);
}