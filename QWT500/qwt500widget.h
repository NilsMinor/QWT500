#ifndef QWT500Widget_H
#define QWT500Widget_H

#include <qwt500widget.h>
#include <QMainWindow>
#include "qwt500.h"

namespace Ui {
class QWT500Widget;
}


#if defined BUILD_DLL
#define TEST_COMMON_DLLSPEC Q_DECL_EXPORT
#else
#define TEST_COMMON_DLLSPEC
#endif

class  TEST_COMMON_DLLSPEC QWT500Widget : public QMainWindow
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

signals:
    void newDataMeasured (mDataHandler * L1,mDataHandler * L2, mDataHandler * L3, mDataHandler * LT);
};

#endif // QWT500Widget_H
