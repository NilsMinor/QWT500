#include "qwt500widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWT500Widget w;
    w.show();

    return a.exec();
}
