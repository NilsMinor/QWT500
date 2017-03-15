#include "qwt500widget.h"
#include "ui_qwt500widget.h"


QWT500Widget::QWT500Widget(QWidget *parent) : QMainWindow(parent),  ui(new Ui::QWT500Widget)
{
    ui->setupUi(this);
    ui->horizontalLayout->addWidget(&wt500);
    wt500.show();
}

QWT500Widget::~QWT500Widget()
{
    delete ui;
}

void QWT500Widget::on_pushButtonSearch_released()
{
     if (wt500.search())
     {
         ui->lineEditDeviceInfo->setText(wt500.getDeviceInfo().at(2));
         ui->pushButtonSearch->setText("Connected");
         ui->pushButtonSearch->setEnabled(false);
         ui->pushButtonReset->setEnabled(true);
         ui->pushButtonStart->setEnabled(true);
         ui->comboBoxPeriod->setEnabled(true);
     }
     else
     {
         ui->lineEditDeviceInfo->setText("Error, no device found!");
     }
}
/*
void QWT500Widget::on_Test_released()
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

void QWT500Widget::on_pushButtonStart_released()
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

void QWT500Widget::on_comboBoxPeriod_currentIndexChanged(const QString &arg1)
{

}

void QWT500Widget::on_pushButtonReset_released()
{

}
