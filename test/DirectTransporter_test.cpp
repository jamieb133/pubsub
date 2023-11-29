
#include <gtest/gtest.h>
#include "DirectTransporter.h"
#include "MockDeliverer.h"
#include "MockTopic.h"

TEST(DirectTransporterTest, send) 
{
    auto deliverer = std::make_shared<MockDeliverer>();
    auto topic = std::make_shared<MockTopic>();
    pubsub::DirectTransporter transporter { deliverer };

    EXPECT_CALL(*deliverer, on_data)
        .Times(testing::Exactly(1));

    transporter.send(topic);
}