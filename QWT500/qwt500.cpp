#include "qwt500.h"

QWT500::QWT500(QWidget *parent) : QWidget(parent), m_layout (NULL)
{
    m_layout = new QHBoxLayout(this);
    m_layout->setContentsMargins(0,0,0,0);
    m_layout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    this->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
    this->setContentsMargins(0,0,0,0);
    this->setLayout(m_layout);
    this->show();

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
   L1Data->addNoErr("U1","Vrms");
   L1Data->addNoErr("I1","Irms");
   L1Data->addNoErr("P1","W");
   L1Data->addNoErr("Q1","VAR");
   L1Data->addNoErr("S1","VA");
   L1Data->addNoErr("λ1","");
   L1Data->addNoErr("EP1","kWh");
   L1Data->addNoErr("EQ1","kWh");
   L1Data->addNoErr("M1","€");
   m_layout->addWidget(L1Data);

   L2Data = new mDataHandler (NULL);
   L2Data->addNoErr("U2","Vrms");
   L2Data->addNoErr("I2","Irms");
   L2Data->addNoErr("P2","W");
   L2Data->addNoErr("2","VAR");
   L2Data->addNoErr("S2","VA");
   L2Data->addNoErr("λ2","");
   L2Data->addNoErr("EP2","kWh");
   L2Data->addNoErr("EQ2","kWh");
   L2Data->addNoErr("M2","€");
   m_layout->addWidget(L2Data);

   L3Data = new mDataHandler (NULL);
   L3Data->addNoErr("U3","Vrms");
   L3Data->addNoErr("I3","Irms");
   L3Data->addNoErr("P3","W");
   L3Data->addNoErr("Q3","VAR");
   L3Data->addNoErr("S3","VA");
   L3Data->addNoErr("λ3","");
   L3Data->addNoErr("EP3","kWh");
   L3Data->addNoErr("EQ3","kWh");
   L3Data->addNoErr("M3","€");
   m_layout->addWidget(L3Data);

   LTData = new mDataHandler (NULL);
   LTData->addNoErr("UT","Vrms");
   LTData->addNoErr("IT","Irms");
   LTData->addNoErr("PT","W");
   LTData->addNoErr("QT","VAR");
   LTData->addNoErr("ST","VA");
   LTData->addNoErr("λT","");
   LTData->addNoErr("EPT","kWh");
   LTData->addNoErr("EQT","kWh");
   LTData->addNoErr("MT","€");
   m_layout->addWidget(LTData);

    /*
   Common = new mDataHandler (this);
   Common->addNoErr("F", "Hz");
   Common->addNoErr("Temp", "°C");
   Common->addNoErr("Circ T", "ms");
   Common->addNoErr("Circ F", "°Hz");
   ui->layoutCommonPanelData->addWidget(Common);
   */
}
int QWT500::send(QString msg)
{
    QByteArray ba = msg.toLatin1();
    return TmcSend( m_iID,(LPSTR)(LPCTSTR) ba.data( ) );
}
float QWT500::receive( int blen, int* rlen)
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

    foreach(float listData, data)
    {
         qDebug() << listData;
    }


    return 5.0;
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

void QWT500::m_timeout()
{
    qDebug() << "... timeout";
    // check if connected

    this->send(":NUMERIC:FORMAT ASCII");
    this->send(":NUMERIC:NORMAL:NUMBER 4");
    this->send(":NUMERIC:NORMAL:ITEM1 URMS,1;ITEM2 URMS,2;ITEM3 IRMS,1;ITEM4 URMS,SIGMA");
    this->send(":NUMERIC:NORMAL:VALUE?");

    int rl = 0;
    this->receive(100,&rl);

    qDebug() << "Real length = " << rl;

    emit newDataAvailable();
}
bool  QWT500::search( void ) {
    DEVICELIST	listbuff[127] ;

    int		num ;
    int		ret ;
    int		chk_wt = 0;

    ret = TmcSearchDevices(TM_CTL_USBTMC2, listbuff,127,&num,NULL); // check for 1 device only
    chk_wt = Check_WTSeries(TM_CTL_USBTMC2, listbuff[0].adr, true);

    qDebug () << m_iID;
    if (chk_wt == 0)
    {
        return true;   // info in deviceInfo
    }

    return false;
}

void QWT500::start (int time)
{
    m_isRunning = true;
    m_timer->start(time);
}
void QWT500::stop()
{
    m_isRunning = false;
    m_timer->stop();
}
