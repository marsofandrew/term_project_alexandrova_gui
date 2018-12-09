#include "stepbystepsimulationlogger.hpp"
#include "library/include/Order.hpp"
#include "library/include/interfaces/GeneratorPool.hpp"
#include "library/include/interfaces/ProcessorPool.hpp"
#include "library/include/interfaces/Processor.hpp"
#include "code/ProcessorImpl.hpp"
#include <code/BufferImpl.hpp>
#include <algorithm>
#include <iostream>


StepByStepSimulationLogger::StepByStepSimulationLogger(const std::shared_ptr<GeneratorPool>& generatorPool,  std::shared_ptr<BufferImpl>& buffer,
                                                       std::shared_ptr<ProcessorPool>& processorPool):
    buffer_(buffer),
    processorPool_(processorPool)
{
}

void StepByStepSimulationLogger::sendCreatedOrder(const std::shared_ptr<Order> &order)
{
    Order ord = *order;
    std::list<Order> savedBuffer;
    std::list<std::shared_ptr<Order>> temp = buffer_->getBufferQueue();
    std::transform(temp.begin(),temp.end(),std::back_inserter(savedBuffer),[](std::shared_ptr<Order> ord){
        return *ord;
    });
    Step s(CREATED,ord,savedBuffer);
    steps.push_back(s);
}

void StepByStepSimulationLogger::sendRefusedOrder(const std::shared_ptr<Order> &order)
{
    Order ord = *order;
    std::list<Order> savedBuffer;
    std::list<std::shared_ptr<Order>> temp = buffer_->getBufferQueue();
    std::transform(temp.begin(),temp.end(),std::back_inserter(savedBuffer),[](std::shared_ptr<Order> ord){
        return *ord;
    });
    Step s(REFUSED,ord,savedBuffer);
    steps.push_back(s);
}

void StepByStepSimulationLogger::sendProcessedOrder(const std::shared_ptr<Order> &order)
{
    Order ord = *order;
    std::list<Order> savedBuffer;
    std::list<std::shared_ptr<Order>> temp = buffer_->getBufferQueue();
    std::transform(temp.begin(),temp.end(),std::back_inserter(savedBuffer),[](std::shared_ptr<Order> ord){
        return *ord;
    });
    Step s(PROCESSED,ord,savedBuffer);
    steps.push_back(s);
}

void StepByStepSimulationLogger::sendBufferedOrder(const std::shared_ptr<Order> &order)
{
    std::list<std::shared_ptr<Order>> temp = buffer_->getBufferQueue();
    std::list<Order> savedBuffer;
    std::transform(temp.begin(),temp.end(),std::back_inserter(savedBuffer),[](std::shared_ptr<Order> ord){
        return *ord;
    });
    Order ord = *order;
    Step s(BUFFERED,ord,savedBuffer);
    steps.push_back(s);

}

void StepByStepSimulationLogger::sendGetOrderFromBuffer(const std::shared_ptr<Order> &order)
{}

void StepByStepSimulationLogger::sendAddingOrderToBuffer(const std::shared_ptr<Order> &order)
{
    std::list<std::shared_ptr<Order>> temp = buffer_->getBufferQueue();
    std::list<Order> savedBuffer;
    std::transform(temp.begin(),temp.end(),std::back_inserter(savedBuffer),[](std::shared_ptr<Order> ord){
        return *ord;
    });
    Order ord = *order;
    Step s(ADDING,ord,savedBuffer);
    steps.push_back(s);
}

void StepByStepSimulationLogger::sendOrderToProcessor(const std::shared_ptr<Order> &order,
     const std::shared_ptr<Processor> &)
{
    std::list<std::shared_ptr<Order>> temp = buffer_->getBufferQueue();
    std::list<Order> savedBuffer;
    std::transform(temp.begin(),temp.end(),std::back_inserter(savedBuffer),[](std::shared_ptr<Order> ord){
        return *ord;
    });
    Order ord = *order;
    Step s(SENTTOPROCESS,ord,savedBuffer);
    steps.push_back(s);
}

void StepByStepSimulationLogger::sendMessage(const Logger::Type &type, const std::string &message)
{}

void StepByStepSimulationLogger::setTimer(const std::shared_ptr<Timer> &timer)
{
    timer_ = timer;

}
std::shared_ptr<StepByStepSimulationLogger::Step> StepByStepSimulationLogger::getStep(int i)
{
    if (i>=steps.size())
    {
        return nullptr;
    }
    return std::make_shared<StepByStepSimulationLogger::Step>(steps.at(i));
}
