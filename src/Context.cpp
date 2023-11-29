#include "Context.h"
#include "ITransporter.h"

using namespace pubsub;

void Context::publish(std::shared_ptr<ITopic> topic)
{
    for(auto const& transporter : mTransporters)
    {
        transporter->send(topic);
    }
}

void Context::add_transporter(std::shared_ptr<ITransporter> transporter)
{
    mTransporters.push_back(transporter);
}

void Context::add_deliverer(std::shared_ptr<IDeliverer> deliverer)
{
    mDeliverers.push_back(deliverer);
}