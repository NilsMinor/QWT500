#include "mdata.h"

mData::mData(QObject *parent, QString nameStr, QString unitStr, bool err)
    : QObject(parent), m_nameStr (nameStr), m_unitStr (unitStr)

{
    m_data = 0;
    m_error = 0;
    m_accuracy = 3;
    m_supportError = err;

    m_labels.append( new QLabel ( m_nameStr));
    m_labels.append( new QLabel ( QString::number(m_data, 'f', m_accuracy ) ) );
    m_labels.append( new QLabel ( m_unitStr) );

    if (m_supportError) {
        m_labels.append( new QLabel ( QString::number(m_error, 'f', 1) ) );
        m_labels[3]->setStyleSheet("QLabel { color : red }");
    }
    m_labels[1]->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    m_layout = new QHBoxLayout ();
    m_widget = new QWidget ( );
    m_layout->setContentsMargins(0,0,0,0);
    m_layout->setSpacing(10);


    foreach (QLabel *l, m_labels) {
       m_layout->addWidget(l,Qt::AlignLeft);
       m_layout->addStretch();
     }

    m_widget->setLayout(m_layout);
}

void mData::print()
{
    qDebug () << m_nameStr << " : " << m_data << " : " << m_unitStr;
}

QWidget *mData::getWidget()
{
    return m_widget;
}
void mData::setData(float data)
{
    m_data = data;
    m_labels[1]->setText( QString::number(m_data, 'f', m_accuracy) ); // update label
}

void mData::setData(float data, float target)
{
    setData (data);
    m_target = target;
    if (m_supportError) {
        m_error = calcError (m_target);
        m_labels[3]->setText( QString::number(m_error, 'f', 1) ); // update label
    }
}
void mData::setAccuracy(int accuracy)
{
    m_accuracy = accuracy;
    this->setData( m_data,m_target ); // update
}
float mData::calcError(float target)
{
    // target = 100%
    // m_data = x
    if (target == 0)
        return 0;

    return (float) 100 - (m_data * 100.0 / target);
}

