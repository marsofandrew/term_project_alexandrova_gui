#include "stepbystepform.h"
#include "ui_stepbystepform.h"
#include <QTableWidget>
#include <QMessageBox>

namespace details{
//StepByStepForm::
    static void onBufferedAction(int step,  int bufferSize_, Ui::StepByStepForm *ui,
               std::shared_ptr<StepByStepSimulationLogger> logger_)
    {
        std::list<Order> temp = logger_->getStep(step)->bufferQueue_;
        std::list<Order>::iterator it;
        for(int i = 0; i < bufferSize_; i++){//ids.size()
            it = std::next(temp.begin(),i);//,N
            if (i<temp.size()) {
                ui->tableBuffer->setItem(i, 0, new QTableWidgetItem(QString::number((*it).getGenerator()->getId())));
                ui->tableBuffer->setItem(i, 1, new QTableWidgetItem(QString::number((*it).getId())));
                ui->tableBuffer->setItem(i, 2, new QTableWidgetItem(QString::number((*it).getGeneratedTime())));
                //TODO buffered output to system state table
                //TODO when senttoprocess call update on buffer
            } else {
                ui->tableBuffer->setItem(i, 0, new QTableWidgetItem("null"));
                ui->tableBuffer->setItem(i, 1, new QTableWidgetItem("null"));
                ui->tableBuffer->setItem(i, 2, new QTableWidgetItem("null"));
            }
            //ui->tableBuffer->setItem(i, 1, new QTableWidgetItem(QString::number(logger->getRefusedProbability(ids[i]))));
        }
    }

    static void onRefusedAction(int step,  int bufferSize_, Ui::StepByStepForm *ui,
               std::shared_ptr<StepByStepSimulationLogger> logger_)
    {
        int tempRow = ui->tableSystem->rowCount();
        //ui->tableSystem->setItem(tempRow, 0, new QTableWidgetItem((*it)->getGenerator()->getId()));
        ui->tableSystem->setItem(tempRow, 1, new QTableWidgetItem("Refuse\n order"));
        //ui->tableBuffer->setItem(i, 1, new QTableWidgetItem(QString::number(logger->getRefusedProbability(ids[i]))));

    }
}
StepByStepForm::StepByStepForm(QWidget *parent,std::shared_ptr<StepByStepSimulationLogger> logger,
                               int amountOfGenerators,int amountOfProcessors,int bufferSize) :
    QDialog(parent),
    ui(new Ui::StepByStepForm),
    logger_(logger),
    amountOfGenerators_(amountOfGenerators),
    amountOfProcessors_(amountOfProcessors),
    bufferSize_(bufferSize),
    stepNum(0)
{

    actions[StepByStepSimulationLogger::BUFFERED]=details::onBufferedAction;
    actions[StepByStepSimulationLogger::REFUSED]=details::onRefusedAction;
    ui->setupUi(this);
    this->setFixedSize(1200,700);
    setUpBufferTable();
    setUpSystemTable();
    setUpProcessorTable();
}

void StepByStepForm::setUpBufferTable()
{
    QStringList horizontalHeader;

    horizontalHeader.append("GenNum");
    horizontalHeader.append("OrderNum");
    horizontalHeader.append("GenTime");
    QStringList verticalHeader;

    for (int i=0; i<bufferSize_;++i)
    {
        verticalHeader.append("BufCell" + QString::number(i));
    }
    ui->tableBuffer->setFixedSize(391,660);
    ui->tableBuffer->setColumnCount(3);
    ui->tableBuffer->setRowCount(bufferSize_);//bufferSize_);//TODO
    ui->tableBuffer->setShowGrid(true);

    ui->tableBuffer->setHorizontalHeaderLabels(horizontalHeader);
    ui->tableBuffer->setVerticalHeaderLabels(verticalHeader);
    ui->tableBuffer->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

void StepByStepForm::setUpSystemTable()
{
    QStringList horizontalHeader;

    horizontalHeader.append("Time");
    horizontalHeader.append("Action");
    horizontalHeader.append("Processor");
    QStringList verticalHeader;

    for (int i=0; i<bufferSize_;++i)
    {
        verticalHeader.append("Step" + QString::number(i));
    }
    ui->tableSystem->setFixedSize(391,660);
    ui->tableSystem->setColumnCount(3);
    ui->tableSystem->setRowCount(amountOfGenerators_);//;//TODO
    ui->tableSystem->setShowGrid(true);

    ui->tableSystem->setHorizontalHeaderLabels(horizontalHeader);
    ui->tableSystem->setVerticalHeaderLabels(verticalHeader);
    ui->tableSystem->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);

}

void StepByStepForm::setUpProcessorTable()
{
    QStringList horizontalHeader;

    horizontalHeader.append("State");
    horizontalHeader.append("TimeStart");
    horizontalHeader.append("TimeFinish");
    horizontalHeader.append("OrderNum");
    QStringList verticalHeader;

    for (int i=0; i<amountOfProcessors_;++i)
    {
        verticalHeader.append("Proc" + QString::number(i));
    }
    ui->tableProcessor->setFixedSize(391,660);
    ui->tableProcessor->setColumnCount(4);
    ui->tableProcessor->setRowCount(amountOfProcessors_);//;//TODO
    ui->tableProcessor->setShowGrid(true);

    ui->tableProcessor->setHorizontalHeaderLabels(horizontalHeader);
    ui->tableProcessor->setVerticalHeaderLabels(verticalHeader);
    ui->tableProcessor->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

StepByStepForm::~StepByStepForm()
{
    delete ui;
}



/*void StepByStepForm::onBufferedAction(int step)
{   std::list<std::shared_ptr<Order>> temp = logger_->getStep(step).bufferQueue_;
    std::list<std::shared_ptr<Order>>::iterator it;
    for(int i = 0; i < bufferSize_; i++){//ids.size()
        it = std::next(temp.begin());//,N
        ui->tableBuffer->setItem(i, 0, new QTableWidgetItem((*it)->getGenerator()->getId()));
        //ui->tableBuffer->setItem(i, 1, new QTableWidgetItem(QString::number(logger->getRefusedProbability(ids[i]))));
    }
}*/


void StepByStepForm::getNextAction(int i)
{
    std::shared_ptr<StepByStepSimulationLogger::Step> step = logger_->getStep(i);
    if (step==nullptr)
    {
        QMessageBox::warning(this, "Конец","Моделирование системы закончено");
        return;
    }
    StepByStepSimulationLogger::Statuses status = (*step).status_;
    actions[status](i,bufferSize_,ui,logger_);
}

void StepByStepForm::on_nextStepButton_clicked()
{
    getNextAction(stepNum++);
    //logger_->ge
   /* for(std::size_t i = 0; i < bufferSize_; i++){//ids.size()
        ui->tableBuffer->setItem(i, 0, new QTableWidgetItem(QString::number(i)));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(logger->getRefusedProbability(ids[i]))));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(logger->getAverageTimeInSystem(ids[i]))));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(logger->getAverageTimeInBuffer(ids[i]))));
        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(logger->getAverageTimeInProcessor(ids[i]))));
        ui->tableWidget->setItem(i, 5, new QTableWidgetItem(QString::number(logger->getBufferDispersion(ids[i]))));
        ui->tableWidget->setItem(i, 6, new QTableWidgetItem(QString::number(logger->getProcessorDispersion(ids[i]))));
    }*/
}
