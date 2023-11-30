#include <functional>
#include <iostream>
#include <string>
#include <atomic>

#include "Context.hpp"
#include "DirectTransporter.h"
#include "PipeTransporter.h"
#include "PipeServer.h"
#include "Deliverer.h"
#include "basic_serialisation.h"
#include "ExampleTopic.h"

using namespace pubsub;

int main()
{
    // Create a deliverer for mapping received topics to subscribers.
    auto deliverer = std::make_shared<Deliverer>();

    // A direct transporter will distribute messages in the same process with no serialisation.
    auto directTransporter = std::make_shared<pubsub::DirectTransporter>(deliverer);

    // Create a serialiser for transporters that require it.
    auto serialiser = std::make_shared<pubsub::basic_serialisation::BasicSerialiser>();
    auto deserialiser = std::make_shared<pubsub::basic_serialisation::BasicDeserialiser>();

    // A pipe transporter with a server will distribute messages across processes, therefore require serialisers.
    std::string const pipename { "pubsub_example_pipe" };
    auto pipeTransporter = std::make_shared<pubsub::PipeTransporter>(pipename, serialiser);
    auto pipeServer = std::make_shared<pubsub::PipeServer>(pipename, deliverer, deserialiser);

    pubsub::Context context{};
    context.add_deliverer(deliverer);
    context.add_transporter(pipeTransporter);
    context.add_server(pipeServer);

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
        std::this_thread::sleep_for(std::chrono::milliseconds{100});
    }
}
