#ifndef QWT500ITEM_H
#define QWT500ITEM_H

#include <QObject>

class qwt500Item : public QObject
{
    Q_OBJECT
public:
    explicit qwt500Item(QObject *parent = 0);
private:
    QString m_functionName;
    QString m_DataName;

signals:

public slots:
};

#endif // QWT500ITEM_H
