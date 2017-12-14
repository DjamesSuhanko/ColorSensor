#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QColorDialog>
#include <QColor>
#include <QDebug>
#include <QThread>
#include "dialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QColorDialog *showColor;
    QSerialPort *serialPort;
    bool isConnected = false;

private:
    Ui::MainWindow *ui;

public slots:
    void connectToSerial();
    void mySerialReader();

signals:
    void startReading(bool ok);
};

#endif // MAINWINDOW_H
