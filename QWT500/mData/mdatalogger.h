#ifndef MDATALOGGER_H
#define MDATALOGGER_H

#include <QObject>
#include <QFile>
#include <QTimer>
#include <QDateTime>
#include "mData/mdata.h"
#include "mData/mdatahandler.h"

class mDataLogger : public QObject
{
    Q_OBJECT
public:
    explicit mDataLogger(QObject *parent = 0);
    void add (mDataHandler * list);
    void enableLogging (int timeout);
    void disableLogging (void);
    void create (QString fileName);
    bool isLogging (void) { return logging; }

private:
    QList <mDataHandler*> mDataHandlerList;
    QTimer              m_timer;
    QString             getTimestamp (void);
    QString             m_fileName;
    bool                    logging;
    QFile                  *m_file;
    QTextStream     *m_stream;
    void                    printHeader(void);
    int                     line_counter;
signals:
    void newDataLogged (int actual_line, qint64 file_size);
public slots:
    void log        (void);
};

#endif // MDATALOGGER_H
