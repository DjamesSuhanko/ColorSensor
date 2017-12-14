#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->serialPort = new QSerialPort;

    QStringList ports;

    //pegar portas disponiveis
    const auto serialPortInfos = QSerialPortInfo::availablePorts();
    //criar uma lista com as portas disponíveis para alimentar o combobox
    int i = 0;
    for (const QSerialPortInfo &serialPortInfo : serialPortInfos) {
        ports.insert(i, serialPortInfo.portName());
        i++;
    }
    //alimentar o combobox com a lista de portas
    ui->comboBox_port->insertItems(0,ports);

    //criar lista de velocidades pro combobox
    QStringList bauds;
    bauds << "9600" << "19200" << "38400" << "57600" << "115200";
    ui->comboBox_baud->insertItems(0,bauds);

    //mostrar o diálogo de cores
    this->showColor = new QColorDialog;
    this->showColor->show();

    connect(this,SIGNAL(startReading(bool)),this,SLOT(mySerialReader()));
    connect(ui->pushButton_connect,SIGNAL(clicked(bool)),this,SLOT(connectToSerial()));
}

void MainWindow::connectToSerial()
{
    if (this->isConnected){
        this->serialPort->close();
        ui->label_status->setText("Não conectado");
        this->isConnected = false;
        ui->pushButton_connect->setText("Conectar");
        return;
    }
    //configurar a porta serial
    this->serialPort->setPortName(ui->comboBox_port->currentText());
    this->serialPort->setBaudRate(ui->comboBox_baud->currentText().toUInt());

    //conectar à porta serial
    if (!this->serialPort->open(QIODevice::ReadOnly)){
        ui->label_status->setText("Falha ao tentar conectar :-(");
        return;
    }
    ui->label_status->setText("Conectado! :-)");
    ui->pushButton_connect->setText("Desconectar");
    this->isConnected = true;
    emit startReading(true);
}

void MainWindow::mySerialReader()
{
    QByteArray readData = this->serialPort->readAll();
    while (this->serialPort->waitForReadyRead(5000)){
        readData = this->serialPort->readAll();
        if (readData.contains("\r\n")){

            readData = readData.replace("\r\n","");
            QStringList TCS;
            TCS << QString(readData).split(" ");
            qDebug() << TCS;
            if (TCS.length() == 3){
                QColor myColor;
                myColor.setBlue(TCS.at(2).toInt());
                myColor.setRed(TCS.at(0).toInt());
                myColor.setGreen(TCS.at(1).toInt());
                this->showColor->setCurrentColor(myColor);
            }

        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
