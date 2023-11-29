#include <functional>
#include <iostream>
#include <string>
#include <atomic>

#include "Context.hpp"
#include "DirectTransporter.h"
#include "Deliverer.h"
#include "ExampleTopic.h"

using namespace pubsub;

int main()
{
    auto deliverer = std::make_shared<Deliverer>();
    auto directTransporter = std::make_shared<pubsub::DirectTransporter>(deliverer);

    pubsub::Context context{};
    context.add_deliverer(deliverer);
    context.add_transporter(directTransporter);

    std::function<void(std::shared_ptr<ExampleTopic> const)> callback 
    {
        [](std::shared_ptr<ExampleTopic> const topic)
        {
            std::cout << "Received " << topic->getMessage() << std::endl;
        }
    };

    context.subscribe(callback);

    for(int i = 0; i < 10; i++)
    {
        auto topic = std::make_shared<ExampleTopic>("hello " + std::to_string(i));
        context.publish(topic);
        std::this_thread::sleep_for(std::chrono::milliseconds{500});
    }
}
