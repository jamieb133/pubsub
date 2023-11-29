#include <gtest/gtest.h>
#include "Context.hpp"
#include "MockTopic.h"
#include "MockDeliverer.h"
#include "MockDistributor.h"

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

    auto distributor = std::make_shared<MockDistributor>();
    auto topic = std::make_shared<MockTopic>();

    EXPECT_CALL(*distributor, send)
        .Times(testing::AtLeast(1));

    context.add_distributor(distributor);
    context.publish(topic);
}
