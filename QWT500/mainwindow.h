#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qwt500.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButtonSearch_released();
    void on_pushButtonStart_released();

private:
    Ui::MainWindow *ui;
    QWT500 wt500;
};

#endif // MAINWINDOW_H
