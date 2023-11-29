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

    auto transporter = std::make_shared<MockTransporter>();
    auto topic = std::make_shared<MockTopic>();

    EXPECT_CALL(*transporter, send)
        .Times(testing::AtLeast(1));

    context.add_transporter(transporter);
    context.publish(topic);
}
