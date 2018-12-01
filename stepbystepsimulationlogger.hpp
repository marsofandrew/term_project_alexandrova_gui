#ifndef STEPBYSTEPSIMULATIONLOGGER_HPP
#define STEPBYSTEPSIMULATIONLOGGER_HPP

#include <library/include/interfaces/Logger.hpp>



class StepByStepSimulationLogger: public Logger
{
public:
    StepByStepSimulationLogger();

    void sendCreatedOrder(const std::shared_ptr<Order> &order) override;
    void sendRefusedOrder(const std::shared_ptr<Order> &order) override;
    void sendProcessedOrder(const std::shared_ptr<Order> &order) override;
    void sendBufferedOrder(const std::shared_ptr<Order> &order) override;
    void sendGetOrderFromBuffer(const std::shared_ptr<Order> &order) override;
    void sendAddingOrderToBuffer(const std::shared_ptr<Order> &order) override;
    void sendOrderToProcessor(const std::shared_ptr<Order> &order, const std::shared_ptr<Processor> &processor) override;
    void sendMessage(const Type &type, const std::string &message) override;
    void setTimer(const std::shared_ptr<Timer> &timer) override;
};

#endif // STEPBYSTEPSIMULATIONLOGGER_HPP
