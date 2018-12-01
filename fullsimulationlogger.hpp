#ifndef FULLSIMULATIONLOGGER_HPP
#define FULLSIMULATIONLOGGER_HPP

#include <memory>
#include "library/include/interfaces/Logger.hpp"
#include "library/include/interfaces/Timer.hpp"
#include <vector>
#include <map>

class Order;
class GeneratorPool;

class FullSimulationLogger: public Logger
{
public:
    FullSimulationLogger(const std::shared_ptr<GeneratorPool>& generatorPool);

    void sendCreatedOrder(const std::shared_ptr<Order> &order) override;

    void sendRefusedOrder(const std::shared_ptr<Order> &order) override;

    void sendProcessedOrder(const std::shared_ptr<Order> &order) override;

    void sendBufferedOrder(const std::shared_ptr<Order> &order) override;

    void sendGetOrderFromBuffer(const std::shared_ptr<Order> &order) override;

    void sendAddingOrderToBuffer(const std::shared_ptr<Order> &order) override;

    void sendOrderToProcessor(const std::shared_ptr<Order> &order,
                                      const std::shared_ptr<Processor> &processor) override;

    void sendMessage(const Type &type, const std::string &message) override;

    void setTimer(const std::shared_ptr<Timer> &timer) override;

    std::vector<unsigned long> getGeneratorsIDs();

    std::size_t getAmountOfCreatedOrders(unsigned long generatorID);

    double getRefusedProbability(unsigned long generatorID);

    Timer::time getAverageTimeInSystem(unsigned long generatorID);

    Timer::time getAverageTimeInProcessor(unsigned long generatorID);

    Timer::time getAverageTimeInBuffer(unsigned long generatorID);

    double getProcessorWorkFactor(unsigned long processorID, Timer::time fullTime);

private:
    std::shared_ptr<Timer> timer_;
    std::map<unsigned long, std::vector<std::shared_ptr<Order>>> cashe_;
};

#endif // FULLSIMULATIONLOGGER_HPP
