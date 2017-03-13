#include "qwt500item.h"

qwt500Item::qwt500Item(QObject *parent, QString functionName, QString dataName, int element, mDataHandler *dataHandler)
    : QObject(parent), m_functionName(functionName), m_DataName(dataName), m_element(element), m_mDataHandler (dataHandler)
{

}
