#include "stepbystepsimulationlogger.hpp"
#include "library/include/Order.hpp"
#include "library/include/interfaces/GeneratorPool.hpp"
#include "library/include/interfaces/ProcessorPool.hpp"
#include "library/include/interfaces/Processor.hpp"
#include <code/BufferImpl.hpp>
#include <algorithm>
#include <iostream>
/*lass Order;
class GeneratorPool;
class ProcessorPool;*/
//class BufferImpl;

StepByStepSimulationLogger::StepByStepSimulationLogger(const std::shared_ptr<GeneratorPool>& generatorPool,  std::shared_ptr<BufferImpl>& buffer,
                                                       std::shared_ptr<ProcessorPool>& processorPool):
    buffer_(buffer),
    processorPool_(processorPool)
{

    /*auto generators = generatorPool->getGenerators();
    for (std::shared_ptr<Generator> generator: generators){
        cache_.insert(std::pair<unsigned long, std::vector<std::shared_ptr<Order>>>(generator->getId(), std::vector<std::shared_ptr<Order>>()));
    }*/


    //bufferSteps_ = new std::vector<std::list<std::shared_ptr<Order>>>();
    /*auto processors = processorPool->getProcessors();
    for (std::shared_ptr<Generator> generator: generators){
        cache_.insert(std::pair<unsigned long, std::vector<std::shared_ptr<Order>>>(generator->getId(), std::vector<std::shared_ptr<Order>>()));
    }*/

}
//std::vector<std::string> statuses = {"Created","Refused","Buffered","SentToProcess","Processed"};
void StepByStepSimulationLogger::sendCreatedOrder(const std::shared_ptr<Order> &order)
{
    //cache_.at(order->getGenerator()->getId()).push_back(order);
}

void StepByStepSimulationLogger::sendRefusedOrder(const std::shared_ptr<Order> &order)
{
    Order ord = *order;
    std::list<Order> savedBuffer;
    std::shared_ptr<Processor> processorsInfo;
    Step s(REFUSED,ord,savedBuffer,processorsInfo);
    steps.push_back(s);
}

void StepByStepSimulationLogger::sendProcessedOrder(const std::shared_ptr<Order> &order)
{}

void StepByStepSimulationLogger::sendBufferedOrder(const std::shared_ptr<Order> &order)
{
    std::cout<<"in";
    std::list<std::shared_ptr<Order>> temp = buffer_->getBufferQueue();
    std::list<Order> savedBuffer;
    std::transform(temp.begin(),temp.end(),std::back_inserter(savedBuffer),[](std::shared_ptr<Order> ord){
        return *ord;
    });
    Order ord = *order;
    std::shared_ptr<Processor> processorsInfo = nullptr;
    int igh=10;
    std::cout<<igh;
    Step s(BUFFERED,ord,savedBuffer,processorsInfo);
    steps.push_back(s);
    int iggh=10;
    std::cout<<iggh;
}

void StepByStepSimulationLogger::sendGetOrderFromBuffer(const std::shared_ptr<Order> &order)
{}

void StepByStepSimulationLogger::sendAddingOrderToBuffer(const std::shared_ptr<Order> &order)
{}

void StepByStepSimulationLogger::sendOrderToProcessor(const std::shared_ptr<Order> &order, const std::shared_ptr<Processor> &processor)
{

}

void StepByStepSimulationLogger::sendMessage(const Logger::Type &type, const std::string &message)
{}

void StepByStepSimulationLogger::setTimer(const std::shared_ptr<Timer> &timer)
{
    timer_ = timer;

}


//void StepByStepSimulationLogger::
