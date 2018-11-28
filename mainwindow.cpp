#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <QMessageBox>

const QString MainWindow::FULL_SIMULATE = "Full simulate";
const QString MainWindow::STEP_BY_STEP_SIMULATE = "Step by step";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    amountOfProcessors_(-1),
    amountOfGenerators_(-1),
    amountOfOrders_(-1),
    minTime_(-1),
    maxTime_(-1),
    lamda_(-1)

{
    ui->setupUi(this);
    ui->comboBox->addItem(FULL_SIMULATE);
    ui->comboBox->addItem(STEP_BY_STEP_SIMULATE);
    ui->fullTimeSimulationScreen->setEnabled(false);
    ui->fullTimeSimulationScreen->setHidden(true);
    this->setFixedSize(800,600);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showCriticalAlert(const QString &message){
    QMessageBox messageBox;
    messageBox.critical(this, "Error", message);
    messageBox.setFixedSize(500,200);
}

bool MainWindow::getVariables(){
    bool ok = false;
    amountOfGenerators_ = ui->tGemeratorAmount_2->text().toInt(&ok);
    if (!ok || amountOfGenerators_ <= 0){
        showCriticalAlert("Amount of generators must be positive integer");
        return false;
    }

   minTime_ = ui->tA_2->text().toDouble(&ok);
    if (!ok || minTime_ < 0){
        showCriticalAlert("A must be a non negative double");
        return false;
    }

    maxTime_ = ui->tB_2->text().toDouble(&ok);
    if (!ok || maxTime_ < 0){
        showCriticalAlert("B must be a non negative double");
        return false;
    }

     amountOfProcessors_ = ui->tProcessorAmount_2->text().toInt(&ok);
    if (!ok || amountOfGenerators_ <= 0){
        showCriticalAlert("Amount of processors must be positive integer");
        return false;
    }

    lamda_ = ui->tLamda_2->text().toDouble(&ok);
    if (!ok || lamda_ < 0){
        showCriticalAlert("Lamda must be a non negative double");
        return false;
    }

   amountOfOrders_ = ui->tOrderAmount_2->text().toInt(&ok);
   if (!ok || amountOfOrders_ <= 0){
       showCriticalAlert("Amount of orders must be positive integer");
       return false;
   }
    bufferSize_ = ui->tBufferSize->text().toInt(&ok);
    if (!ok || bufferSize_ < 0){
       showCriticalAlert("Buffer size must be positive integer or 0");
       return false;
   }
    return true;
}

void MainWindow::runFullSimulation(){
    showCriticalAlert("Full time");
}

void MainWindow::runStepByStepSimulation(){
    showCriticalAlert("Step by step");
}

void MainWindow::on_bSimulate_pressed()
{
    if (getVariables()){
        if (ui->comboBox->currentText() == FULL_SIMULATE){
            runFullSimulation();
        } else if (ui->comboBox->currentText() == STEP_BY_STEP_SIMULATE){
            runStepByStepSimulation();
        }
    }
}
