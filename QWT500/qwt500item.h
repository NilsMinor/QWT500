#ifndef QWT500ITEM_H
#define QWT500ITEM_H

/**
 * @file        qwt500item.h
 * @author  Nils Minor (www.nilsminor.de)
 * @date     29.03.2017
 * @brief     Data Item for easier communication
 *
 * This class is used to simplify the "measurement" process, moreover on how to get data out of the device.
 */

#include <QObject>
#include "mData/mdatahandler.h"

class qwt500Item : public QObject
{
    Q_OBJECT
public:
    explicit qwt500Item (QObject *parent = 0, QString functionName ="", QString dataName = "", int element = 0,
                                        mDataHandler * dataHandler = NULL);
    QString     getFunctionName (void) { return m_functionName; }
    QString     getDataName (void) { return m_DataName; }
    int                 getElement (void) { return m_element; }
    mDataHandler * getHandle (void) { return m_mDataHandler; }

private:
    QString m_functionName;
    QString m_DataName;
    int m_element;
    mDataHandler * m_mDataHandler;

signals:

public slots:
};

#endif // QWT500ITEM_H
