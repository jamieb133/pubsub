#include "Context.h"
#include "IDistributor.h"

using namespace pubsub;

void Context::publish(std::shared_ptr<ITopic> topic)
{
    for(auto const& distributor : mDistributors)
    {
        distributor->send(topic);
    }
}

void Context::add_distributor(std::shared_ptr<IDistributor> distributor)
{
    mDistributors.push_back(distributor);
}

void Context::add_deliverer(std::shared_ptr<IDeliverer> deliverer)
{
    mDeliverers.push_back(deliverer);
}