#ifndef QWT500Widget_H
#define QWT500Widget_H

/**
 * @file        qwt500widget.h
 * @author  Nils Minor (www.nilsminor.de)
 * @date     29.03.2017
 * @brief     Builds a widget to reuse or display it on a gui
 *
 * This class includes the qwt500 driver class. It also controls (not handles) the communication between
 * The computer and the WT500 and emits a signal with new data everytime it is available.
 */

#include <qwt500widget.h>
#include <QMainWindow>
#include "qwt500.h"
#include <QMessageBox>

namespace Ui {
class QWT500Widget;
}


class  QWT500Widget : public QMainWindow
{
    Q_OBJECT

public:
    explicit QWT500Widget(QWidget *parent = 0);
    ~QWT500Widget();

private slots:
    void on_pushButtonQWT500Search_released();
    void on_pushButtonQWT500Start_released();
    void on_pushButtonQWT500Reset_released();
    void on_comboBoxQWT500CurrentRange_currentIndexChanged(const QString &arg1);
    void on_comboBoxQWT500VoltageRange_currentIndexChanged(const QString &arg1);
    void on_comboBoxQWT500Period_currentIndexChanged(const QString &arg1);
    void on_pushButtonInfo_released();

private:
    Ui::QWT500Widget *ui;
    QWT500 wt500;

private slots:
    void newDataAvailable (void);

    void on_comboBoxQWT500Style_currentIndexChanged(int index);

signals:
    void newDataMeasured (mDataHandler * L1,mDataHandler * L2, mDataHandler * L3, mDataHandler * LT);
};

#endif // QWT500Widget_H




