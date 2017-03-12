#include "mdatalogger.h"

mDataLogger::mDataLogger(QObject *parent) : QObject(parent)
{
    m_file = NULL;
    m_stream = NULL;
    logging = false;
    line_counter = 0;
    m_timer.setTimerType(Qt::PreciseTimer);
    connect (&m_timer, SIGNAL(timeout()), this, SLOT(log()));
}

void mDataLogger::add(mDataHandler *list)
{
    mDataHandlerList.append(list);
}
void mDataLogger::enableLogging(int timeout)
{
    m_timer.start(timeout * 1000);
    logging = true;
}
void mDataLogger::disableLogging(void)
{
    logging = false;
    m_file->close();
    m_file = NULL;
    m_stream = NULL;
    m_timer.stop();
    emit newDataLogged(line_counter, m_file->size());
}
void mDataLogger::create(QString fileName)
{
    QDateTime dateTime = dateTime.currentDateTime();
    m_fileName = fileName;
    m_fileName.append("_" + dateTime.toString("yyyy-MM-dd hh-mm-ss"));
    m_fileName.append(".csv");
    m_file = new QFile (m_fileName);

    if (m_file->open(QFile::WriteOnly | QFile::Truncate))
    {
         m_stream = new QTextStream (m_file);
         line_counter = 0;
         this->printHeader();
    }
}
QString mDataLogger::getTimestamp()
{
    QDateTime dateTime = dateTime.currentDateTime();
    //return dateTime.toString("yyyy-MM-dd hh:mm:ss");
    return dateTime.toString("hh:mm:ss");
}

void mDataLogger::printHeader(void)
{
    if (m_file != NULL && m_stream != NULL) {

       if (mDataHandlerList.size() != 5) return;   // L1, L2, L3, LT, Common

       QDateTime dateTime = dateTime.currentDateTime();
        *m_stream <<  dateTime.toString("yyyy-MM-dd") << ";"  ;
        for (int i = 0; i!=  mDataHandlerList[0]->getMDataList().size(); i++)   // all lists must have the same size!
        {
             *m_stream  << mDataHandlerList[0]->getMDataList().at(i)->getPrettyName() << ";"     // L1
                                 << mDataHandlerList[1]->getMDataList().at(i)->getPrettyName() << ";"     // L2
                                 << mDataHandlerList[2]->getMDataList().at(i)->getPrettyName() << ";"     // L3
                                 << mDataHandlerList[3]->getMDataList().at(i)->getPrettyName() << ";";    // LT
        }
        *m_stream << mDataHandlerList[4]->getMDataList().at(0)->getPrettyName() << ";";        // Line Frequency
        *m_stream << mDataHandlerList[4]->getMDataList().at(1)->getPrettyName() << ";";        // Module Temperature
        *m_stream << mDataHandlerList[4]->getMDataList().at(2)->getPrettyName() << ";";        // Circulation Time

        *m_stream  << "\r\n";   // linefeed
    }
    else
    {
        qDebug() << "Log error m_file = " << m_file << " m_stream = " << m_stream;
    }
}
void mDataLogger::log(void)
{
    if (m_file != NULL && m_stream != NULL && logging) {

       if (mDataHandlerList.size() != 5) return;   // L1, L2, L3, LT, Common

        *m_stream << getTimestamp() << ";"   ;  // log timestamp
        for (int i = 0; i!=  mDataHandlerList[0]->getMDataList().size(); i++)   // all lists must have the same size!
        {
             *m_stream  << mDataHandlerList[0]->getMDataList().at(i)->getData() << ";"     // L1
                                 << mDataHandlerList[1]->getMDataList().at(i)->getData() << ";"     // L2
                                 << mDataHandlerList[2]->getMDataList().at(i)->getData() << ";"     // L3
                                 << mDataHandlerList[3]->getMDataList().at(i)->getData() << ";";    // LT
        }
        *m_stream   << mDataHandlerList[4]->getData("F") << ";";                                     // Line Frequency
        *m_stream  << mDataHandlerList[4]->getData("Temp") << ";";                               // Module Temperature
        *m_stream  << mDataHandlerList[4]->getData("Circ T") << ";";                              // Circulation Time

        *m_stream  << "\r\n";   // linefeed
        line_counter++;
        emit newDataLogged(line_counter, m_file->size());
    }
    else
    {
        qDebug() << "Log error m_file = " << m_file << " m_stream = " << m_stream;
    }
}
