#ifndef QWT500_H
#define QWT500_H

/**
 * @file        qwt500.h
 * @author  Nils Minor (www.nilsminor.de)
 * @date     29.03.2017
 * @brief     Basic communication class using tmctl
 *
 * This clas is the main class of QWT500. It uses tmctl to access the (in my case) WT500 of Yokogawa.
 * I've used the example code from their homepage for Visual Studio and simplified the process of communication.
 * I also changed the CString Stuff to QString for easier Handling.
 *
 * the whole datamanagement is based on a data-class I've wrote for the PSOM project.
 * It heavily abstracts the data into the mData, mDatahandler and mDataLogger
 */

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

    int send(QString msg);
    QList <float> receive( int blen, int* rlen);

    void disconnect (void);
    bool search(void);
    void start (int time);
    void stop (void);
    void reset (void);
    void setUpdateRate (QString rate);
    void setVoltageRange (QString range);
    void setCurrentRange (QString range);

    QStringList getDeviceInfo (void) { return deviceInfo; }
    mDataHandler* getPhaseInformation(int phase);
    QStringList getUpdateRateHandle (void) { return UpdateRate; }
    bool isRunning (void) { return m_isRunning; }

private:
    void addItem (QString functionName, mDataHandler * phase, int element, QString mDataName, QString mDataUnit);
    int Check_WTSeries(int wire, char* addr, bool stayConnected);
    bool connected;
    void updateData (void);
    void triggerDataRead (void);
    void resolveReceivedData (void);
    QStringList deviceInfo;
    QStringList UpdateRate;
    int m_iID;
    bool m_isRunning;
    QTimer *m_timer;
    QHBoxLayout *m_layout;
    QList <qwt500Item*> m_itemList;
    bool deviceReady;

    mDataHandler * L1Data;
    mDataHandler * L2Data;
    mDataHandler * L3Data;
    mDataHandler * LTData;
    mDataHandler * Harmonics;

signals:
    void newDataAvailable (void);
private slots:
    void m_timeout (void);
public slots:
};

#endif // QWT500_H
