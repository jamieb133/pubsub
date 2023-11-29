#pragma once

#include <memory>
#include <functional>
#include <vector>

namespace pubsub
{
    class ITopic;
    class IDistributor;
    class IDeliverer;

    class Context
    {
    private:
        std::vector<std::shared_ptr<IDeliverer>> mDeliverers{};
        std::vector<std::shared_ptr<IDistributor>> mDistributors{};
    public:
        void publish(std::shared_ptr<ITopic> topic);
        template <class TopicType> 
        void subscribe(std::function<void(std::shared_ptr<TopicType>)> callback);
        void add_distributor(std::shared_ptr<IDistributor> distributor);
        void add_deliverer(std::shared_ptr<IDeliverer> deliverer);
    };
}
