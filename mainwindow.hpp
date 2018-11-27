#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_bSimulate_pressed();

private:
    Ui::MainWindow *ui;

    int amountOfProcessors_;
    int amountOfGenerators_;
    int amountOfOrders_;
    double minTime_;
    double maxTime_;
    double lamda_;

    const static QString FULL_SIMULATE;
    const static QString STEP_BY_STEP_SIMULATE;

    void showCriticalAlert(const QString &message);
    bool getVariables();

    void runStepByStepSimulation();
    void runFullSimulation();
};

#endif // MAINWINDOW_HPP
