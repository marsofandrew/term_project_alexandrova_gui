#include "fullsimulationlogger.hpp"
#include <utility>
#include <memory>
#include <algorithm>
#include <iterator>
#include "library/include/Order.hpp"
#include "library/include/interfaces/GeneratorPool.hpp"

FullSimulationLogger::FullSimulationLogger(const std::shared_ptr<GeneratorPool>& generatorPool)
{
   auto generators = generatorPool->getGenerators();
   for (std::shared_ptr<Generator> generator: generators){
       cashe_.insert(std::pair<unsigned long, std::vector<std::shared_ptr<Order>>>(generator->getId(), std::vector<std::shared_ptr<Order>>()));
   }
}

void FullSimulationLogger::sendCreatedOrder(const std::shared_ptr<Order> &order)
{
    cashe_.at(order->getGenerator()->getId()).push_back(order);
}

void FullSimulationLogger::sendRefusedOrder(const std::shared_ptr<Order> &order)
{}

void FullSimulationLogger::sendProcessedOrder(const std::shared_ptr<Order> &order)
{}

void FullSimulationLogger::sendBufferedOrder(const std::shared_ptr<Order> &order)
{}

void FullSimulationLogger::sendGetOrderFromBuffer(const std::shared_ptr<Order> &order)
{}

void FullSimulationLogger::sendAddingOrderToBuffer(const std::shared_ptr<Order> &order)
{}

void FullSimulationLogger::sendOrderToProcessor(const std::shared_ptr<Order> &order, const std::shared_ptr<Processor> &processor)
{}

void FullSimulationLogger::sendMessage(const Logger::Type &type, const std::string &message)
{}

void FullSimulationLogger::setTimer(const std::shared_ptr<Timer> &timer)
{
    timer_ = timer;
}

std::size_t FullSimulationLogger::getAmountOfCreatedOrders(unsigned long generatorID)
{
    return cashe_.at(generatorID).size();
}

double FullSimulationLogger::getRefusedProbability(unsigned long generatorID)
{
    auto vector = cashe_.at(generatorID);
    
    int amountOfRefusedOrders = std::count_if(vector.begin(), vector.end(), [](const std::shared_ptr<Order> &order){
        return order->getProcessor() == nullptr;
    });

    return static_cast<double>(amountOfRefusedOrders)/ getAmountOfCreatedOrders(generatorID);
}

Timer::time FullSimulationLogger::getAverageTimeInSystem(unsigned long generatorID)
{
    auto vector = cashe_.at(generatorID);
    std::vector<Timer::time> times;
    std::transform(vector.begin(),vector.end(), std::back_inserter(times), [](const std::shared_ptr<Order> &order){
        if (order->getProcessor()){
            return order->getProcessTime() + order->getStartProcessTime() - order->getGeneratedTime();
        }
        return order->getRefusedTime() - order->getGeneratedTime();
    });
    Timer::time fullTime = std::accumulate(times.begin(), times.end(), 0);
    return fullTime/getAmountOfCreatedOrders(generatorID);
}

Timer::time FullSimulationLogger::getAverageTimeInProcessor(unsigned long generatorID)
{
    auto vector = cashe_.at(generatorID);
    Timer::time timeInProcessor = 0;
    std::size_t amountOfOrders = 0;

    for (std::shared_ptr<Order> order: vector){
        if (order->getProcessor() != nullptr){
            timeInProcessor+=order->getProcessTime();
            amountOfOrders++;
        }
    }

   if (amountOfOrders == 0){
        return 0;
   }
   return timeInProcessor / amountOfOrders;
}

Timer::time FullSimulationLogger::getAverageTimeInBuffer(unsigned long generatorID)
{
    auto vector = cashe_.at(generatorID);
    std::vector<Timer::time> times;
    std::transform(vector.begin(), vector.end(), std::back_inserter(times), [](const std::shared_ptr<Order> &order){
        return (order->getProcessor() != nullptr)? order->getGettingTime() - order->getGeneratedTime(): order->getRefusedTime() - order->getGeneratedTime();
    });
    return std::accumulate(times.begin(),times.end(),0) / static_cast<Timer::time>(getAmountOfCreatedOrders(generatorID));
}
