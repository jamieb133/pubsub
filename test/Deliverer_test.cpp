#include <gtest/gtest.h>
#include "Deliverer.h"
#include "MockTopic.h"

TEST(DelivererTest, ReceiveTopic) 
{
    pubsub::Deliverer deliverer{};
    auto topic = std::make_shared<MockTopic>();

    bool called {false};
    auto callback = [&called](std::shared_ptr<pubsub::ITopic>)
    {
        called = true;
    };

    deliverer.register_subscriber("topicName", callback);
    deliverer.on_data("topicName", topic);
    EXPECT_EQ(called, true);
}