#include "qwt500widget.h"
#include "ui_qwt500widget.h"


QWT500Widget::QWT500Widget(QWidget *parent) : QMainWindow(parent),  ui(new Ui::QWT500Widget)
{
    ui->setupUi(this);
    ui->qwt500HorizontalLayout->addWidget(&wt500);
    wt500.show();
    connect (&wt500, SIGNAL(newDataAvailable()), this, SLOT(newDataAvailable()));
}
QWT500Widget::~QWT500Widget()
{
    delete ui;
}

void QWT500Widget::on_pushButtonQWT500Search_released()
{
     if (wt500.search())
     {
         ui->lineEditQWT500DeviceInfo->setText(wt500.getDeviceInfo().at(2));
         ui->pushButtonQWT500Search->setText("Connected");
         ui->pushButtonQWT500Search->setEnabled(false);
         ui->pushButtonQWT500Reset->setEnabled(true);
         ui->pushButtonQWT500Start->setEnabled(true);
         ui->comboBoxQWT500Period->setEnabled(true);
     }
     else
     {
         ui->lineEditQWT500DeviceInfo->setText("Error, no device found!");
     }
}
void QWT500Widget::on_pushButtonQWT500Start_released()
{
    if (wt500.isRunning()) {
        wt500.stop();
        qDebug() << "Timer stopped";
        ui->pushButtonQWT500Start->setText("Start");
        ui->pushButtonQWT500Start->update();
    }
    else {
        wt500.start(ui->comboBoxQWT500Period->currentText().toInt() / 2);
        ui->pushButtonQWT500Start->setText("Stop");
        ui->pushButtonQWT500Start->update();
        qDebug() << "Timer started";
    }
}
void QWT500Widget::on_pushButtonQWT500Reset_released()
{
    wt500.reset ();
}

void QWT500Widget::on_comboBoxQWT500VoltageRange_currentIndexChanged(const QString &arg1)
{
    wt500.setVoltageRange(arg1);
}
void QWT500Widget::on_comboBoxQWT500CurrentRange_currentIndexChanged(const QString &arg1)
{
    wt500.setCurrentRange(arg1);
}
void QWT500Widget::on_comboBoxQWT500Period_currentIndexChanged(const QString &arg1)
{
    wt500.setUpdateRate(arg1);
}

void QWT500Widget::on_pushButtonInfo_released()
{

}

void QWT500Widget::newDataAvailable()
{
    emit newDataMeasured( wt500.getPhaseInformation(1),
                                                wt500.getPhaseInformation(2),
                                                wt500.getPhaseInformation(3),
                                                wt500.getPhaseInformation(4));
}
