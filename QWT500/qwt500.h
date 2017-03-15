#ifndef QWT500_H
#define QWT500_H

#include <QObject>
#include <QDebug>
#include <cstring>
#include <atlstr.h>
#include <QTimer>
#include "mData/mdatahandler.h"
#include "qwt500item.h"
#include "yokogawa/tmctl.h"
#include "mData/mdatahandler.h"

class QWT500 : public QWidget
{
    Q_OBJECT
public:
    explicit QWT500(QWidget *parent = 0);
    QStringList getDeviceInfo (void) { return deviceInfo; }

    int send(QString msg);
    QList <float> receive( int blen, int* rlen);
    void disconnect (void);
    bool search(void);
    void start (int time);
    void stop (void);
    QStringList getUpdateRateHandle (void) { return UpdateRate; }
    bool isRunning (void) { return m_isRunning; }
    void addItem (QString functionName, mDataHandler * phase, int element, QString mDataName, QString mDataUnit);

private:
    bool connected;
    int Check_WTSeries(int wire, char* addr, bool stayConnected);
    void updateData (void);
    void resolveReceivedData (void);
    QStringList deviceInfo;
    int m_iID;
    QStringList UpdateRate;
    bool m_isRunning;
    QTimer *m_timer;
    QHBoxLayout *m_layout;
    QList <qwt500Item*> m_itemList;


    mDataHandler * L1Data;
    mDataHandler * L2Data;
    mDataHandler * L3Data;
    mDataHandler * LTData;

signals:
    void newDataAvailable (void);
private slots:
    void m_timeout (void);
public slots:
};

#endif // QWT500_H
