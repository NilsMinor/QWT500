#include "qwt500.h"

QWT500::QWT500(QWidget *parent) : QWidget(parent), m_layout (NULL)
{
    connected = false;
    m_layout = new QHBoxLayout(NULL);
    m_layout->setContentsMargins(0,0,0,0);
    m_layout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    this->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
    this->setContentsMargins(0,0,0,0);
    this->setLayout(m_layout);

   UpdateRate.append("100ms");
   UpdateRate.append("200ms");
   UpdateRate.append("500ms");
   UpdateRate.append("1s");
   UpdateRate.append("2s");
   UpdateRate.append("5s");

   m_isRunning = false;
   m_timer = new QTimer (NULL);
   m_timer->setTimerType(Qt::PreciseTimer);
   connect(m_timer, SIGNAL(timeout()), this, SLOT(m_timeout()));

   L1Data = new mDataHandler (this);
   L2Data = new mDataHandler (this);
   L3Data = new mDataHandler (this);
   LTData = new mDataHandler (this);

   //m_layout->addWidget(LTData);

   addItem("URMS",       L1Data, 1, "U1","Vrms");
   addItem("IRMS",         L1Data, 1, "I1","Arms");
   addItem("P",               L1Data, 1, "P1","W");
   addItem("Q",              L1Data, 1, "Q1","VAR");
   addItem("S",               L1Data, 1, "S1","VA");
   addItem("LAMBDA"   ,L1Data, 1, "L1","");
   addItem("WHP",        L1Data, 1, "EP1","kWh");
   addItem("WQ",          L1Data, 1, "EQ1","kWh");

   addItem("URMS",       L2Data, 2, "U2","Vrms");
   addItem("IRMS",         L2Data, 2, "I2","Arms");
   addItem("P",               L2Data, 2, "P2","W");
   addItem("Q",              L2Data, 2, "Q2","VAR");
   addItem("S",               L2Data, 2, "S2","VA");
   addItem("LAMBDA"   ,L2Data, 2, "L2","");
   addItem("WHP",        L2Data, 2, "EP2","kWh");
   addItem("WQ",          L2Data, 2, "EQ2","kWh");

   addItem("URMS",       L3Data, 3, "U3","Vrms");
   addItem("IRMS",         L3Data, 3, "I3","Arms");
   addItem("P",               L3Data, 3, "P3","W");
   addItem("Q",              L3Data, 3, "Q3","VAR");
   addItem("S",               L3Data, 3, "S3","VA");
   addItem("LAMBDA"   ,L3Data, 3, "L3","");
   addItem("WHP",        L3Data, 3, "EP3","kWh");
   addItem("WQ",          L3Data, 3, "EQ3","kWh");

   m_layout->addWidget(L1Data);
   m_layout->addWidget(L2Data);
   m_layout->addWidget(L3Data);

   this->show();

}
int QWT500::send(QString msg)
{
    QByteArray ba = msg.toLatin1();
    //qDebug() << "send : " << msg;
    return TmcSend( m_iID,(LPSTR)(LPCTSTR) ba.data( ) );
}
QList<float> QWT500::receive( int blen, int* rlen)
{
    char rec[1000] = {0};

    TmcReceive (m_iID, rec, blen, rlen);
    QString str(rec);
    QStringList list = str.split(",");
    QList <float> data;

    foreach(QString listString, list)
    {
        if (listString.contains("nan"))
            data.append ( 0 );
        else
            data.append ( listString.toFloat() );
    }

    return data;
}

void QWT500::disconnect()
{
    TmcFinish(m_iID);
}
int   QWT500::Check_WTSeries(int wire,char* addr, bool stayConnected) {

    int ret;

    ret = TmcInitialize(wire,addr, &m_iID);
    if(ret != 0)
    {
        qDebug() << "TmcInitialize error ";
        return ret;
    }

    // tmcsetterm
    ret = TmcSetTerm(m_iID, 2, 1);
    if(ret != 0)
    {
        qDebug() << "TmcSetTerm error ";
        TmcFinish(m_iID);
        return ret;
    }

    // tmcsettimeout(1*100ms)
    ret = TmcSetTimeout(m_iID, 1);
    if(ret != 0)
    {
        qDebug() << "TmcFinish error ";
        TmcFinish(m_iID);
        return ret;
    }

    // test the device module connected.
    ret = TmcSend(m_iID,(char*) "*IDN?");
    int maxLength = 256;
    char* buf ;
    if ((buf = new char[maxLength]) == NULL)
    {
        qDebug() << "TmcSetTimeout error ";
        TmcSetTimeout( m_iID, 20 );
        TmcFinish(m_iID);
        return 1;
    }

    // receive data
    memset(buf,0,maxLength);
    int realLength;
    ret = TmcReceive(m_iID, buf, maxLength, &realLength);

    QString deviceInfoString (buf);
    deviceInfo = deviceInfoString.split(",");
    qDebug() << deviceInfo;

    delete[] buf;

    if(ret != 0)
    {
        TmcFinish(m_iID);
    }

    //tmcsettimeout(20*100ms)
    TmcSetTimeout( m_iID, 20 );
    if (!stayConnected)
        this->disconnect();

    return ret;
}
void QWT500::resolveReceivedData(void)
{
    // put in reception mode
    int rl = 0;
    QList <float> data = receive(1000,&rl);

    if (data.size() != m_itemList.size()) {
        int error = data.first();

        switch (error) {
            case 0: deviceReady = false;
                break;
            case 1: deviceReady = true;
                break;
        default:

            break;
        }
        return ;
    }
    int counter = 0;
    foreach (qwt500Item * item, m_itemList) {
        item->getHandle()->setData(item->getDataName(),data.at(counter++)); // set data of mData element
    }
}

void QWT500::updateData()
{
        if (deviceReady) {
            this->send(":NUMERIC:NORMAL:VALUE?");
            deviceReady = false;
        }
        else {
            this->send(":STATUS:EESR?");
        }
}
void QWT500::triggerDataRead()
{
    this->send(":NUMERIC:FORMAT ASCII");            // send format
    this->send(":NUMERIC:NORMAL:NUMBER " + QString::number(m_itemList.size())); // send data
    QString msgToSend = "";
    int counter = 1;
    foreach (qwt500Item * item, m_itemList) {
        msgToSend.append("ITEM" + QString::number(counter++));
        msgToSend.append(" " + item->getFunctionName());
        msgToSend.append("," + QString::number(item->getElement()));

        if (counter <= m_itemList.size()) {
            msgToSend.append(";");
        }
    }

    this->send(":NUMERIC:NORMAL:" + msgToSend);

    this->send(":STATUS:FILTER1 FALL");
}
void QWT500::m_timeout()
{
    /* send get update
        :NUMERIC:FORMAT ASCII
        :NUMERIC:NORMAL:NUMBER 10
        :NUMERIC:NORMAL:ITEM1 URMS,1;ITEM2 IRMS,1;ITEM3 P,1;ITEM4 Q,1;ITEM5 S,1;ITEM6 LAMBDA,1;ITEM7 WHP,1;ITEM8 WQ,1;ITEM9 URMS,2;ITEM10 IRMS,2
        :STATUS:FILTER1 FALL
        :STATUS:EESR?
        :STATUS:EESR?
        :NUMERIC:NORMAL:VALUE?
        :STATUS:EESR?
        :STATUS:EESR?
        :STATUS:EESR?
        :STATUS:EESR?
        :NUMERIC:NORMAL:VALUE?

        receive

        1
        1
        198.36E+00,409.00E-03,81.13E+00,-0.61E+00,81.13E+00,1.0000E+00,NAN,NAN,0.00E+00,0.0000E+00
        0
        0
        0
        1
        198.33E+00,408.96E-03,81.11E+00,-0.60E+00,81.11E+00,1.0000E+00,NAN,NAN,0.00E+00,0.0000E+00
    */

    updateData ();
    resolveReceivedData ();

    emit newDataAvailable();
}
bool QWT500::search( void ) {
    DEVICELIST	listbuff[127] ;

    int		num ;
    int		ret ;
    int		chk_wt = 0;

    ret = TmcSearchDevices(TM_CTL_USBTMC2, listbuff,127,&num,NULL); // check for 1 device only
    chk_wt = Check_WTSeries(TM_CTL_USBTMC2, listbuff[0].adr, true);

    qDebug () << m_iID;
    if (chk_wt == 0)
    {
        connected = true;
        return true;   // info in deviceInfo
    }

    return false;
}
void QWT500::start (int time)
{
    if (connected) {
        m_isRunning = true;
        m_timer->start(time);
        triggerDataRead();
    }
    else
    {
        // error message
    }
}
void QWT500::stop()
{
    m_isRunning = false;
    m_timer->stop();
}

void QWT500::reset()
{
     this->send("*RST");
}
void QWT500::setUpdateRate(QString rate)
{
    this->send(":RATE " + rate);
}
void QWT500::setVoltageRange(QString range)
{
    this->send(":INPUT:VOLTAGE:RANGE:ELEMENT1 " + range);
    this->send(":INPUT:VOLTAGE:RANGE:ELEMENT2 " + range);
    this->send(":INPUT:VOLTAGE:RANGE:ELEMENT3 " + range);
}

void QWT500::setCurrentRange(QString range)
{
    this->send(":INPUT:CURRENT:RANGE:ELEMENT1 " + range);
    this->send(":INPUT:CURRENT:RANGE:ELEMENT2 " + range);
    this->send(":INPUT:CURRENT:RANGE:ELEMENT3 " + range);
}
void QWT500::addItem(QString functionName, mDataHandler *phase, int element, QString mDataName, QString mDataUnit)
{
    phase->addNoErr(mDataName, mDataUnit);
    qwt500Item * item = new qwt500Item (NULL, functionName, mDataName, element, phase);
    m_itemList.append(item);
}

mDataHandler *QWT500::getPhaseInformation(int phase)
{
    switch (phase) {
        case 1: return L1Data;  break;
        case 2: return L2Data;  break;
        case 3: return L3Data;  break;
        case 4: return LTData;  break;
        default: qDebug() << "getPhaseInformation error for value: " << phase ;
    }
}

