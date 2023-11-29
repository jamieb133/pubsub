#include <gtest/gtest.h>
#include "Context.hpp"
#include "MockTopic.h"
#include "MockDeliverer.h"
#include "MockTransporter.h"

using namespace testing;

TEST(ContextTest, subscribe) 
{
    pubsub::Context context{};

    auto deliverer = std::make_shared<MockDeliverer>();

    EXPECT_CALL(*deliverer, register_subscriber)
        .Times(testing::AtLeast(1));

    context.add_deliverer(deliverer);

    std::function<void(std::shared_ptr<MockTopic>)> callback = [](std::shared_ptr<MockTopic> topic){};
    context.subscribe(callback);
}

TEST(ContextTest, publish) 
{
    pubsub::Context context{};
    auto topic = std::make_shared<MockTopic>();

    auto transporter1 = std::make_shared<MockTransporter>();
    auto transporter2 = std::make_shared<MockTransporter>();
    auto transporter3 = std::make_shared<MockTransporter>();

    EXPECT_CALL(*transporter1, send)
        .Times(testing::Exactly(1));
    EXPECT_CALL(*transporter2, send)
        .Times(testing::Exactly(1));
    EXPECT_CALL(*transporter3, send)
        .Times(testing::Exactly(1));

    context.add_transporter(transporter1);
    context.add_transporter(transporter2);
    context.add_transporter(transporter3);
    context.publish(topic);
}
