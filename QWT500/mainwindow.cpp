#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->horizontalLayout->addWidget(&wt500);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonSearch_released()
{
     if (wt500.search())
     {
         ui->lineEditDeviceInfo->setText(wt500.getDeviceInfo().at(2));
         ui->pushButtonSearch->setText("Connected");
         ui->pushButtonSearch->setEnabled(false);
     }
     else
     {
         ui->lineEditDeviceInfo->setText("Error, no device found!");
     }
}
/*
void MainWindow::on_Test_released()
{
    //wt500.send(":RATE 500ms");
    wt500.send(":NUMERIC:FORMAT ASCII");
    wt500.send(":NUMERIC:NORMAL:NUMBER 4");
    wt500.send(":NUMERIC:NORMAL:ITEM1 URMS,1;ITEM2 URMS,2;ITEM3 IRMS,1;ITEM4 URMS,SIGMA");     // check ; at the end !
    wt500.send(":NUMERIC:NORMAL:VALUE?");

    int rl = 0;
    wt500.receive(100,&rl);

    qDebug() << "Real length = " << rl;

}*/

void MainWindow::on_pushButtonStart_released()
{
    if (wt500.isRunning()) {
        wt500.stop();
        qDebug() << "Timer stopped";
    }
    else {
        wt500.start(ui->comboBoxPeriod->currentText().toInt());
        qDebug() << "Timer started";
    }
}
