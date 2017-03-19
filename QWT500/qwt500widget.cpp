#include "qwt500widget.h"
#include "ui_qwt500widget.h"


QWT500Widget::QWT500Widget(QWidget *parent) : QMainWindow(parent),  ui(new Ui::QWT500Widget)
{
    ui->setupUi(this);
    ui->qwt500HorizontalLayout->addWidget(&wt500);
    wt500.show();
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
    }
    else {
        wt500.start(ui->comboBoxQWT500Period->currentText().toInt());
        qDebug() << "Timer started";
    }
}
void QWT500Widget::on_comboBoxQWT500Period_currentIndexChanged(const QString &arg1)
{

}
void QWT500Widget::on_pushButtonQWT500Reset_released()
{

}
