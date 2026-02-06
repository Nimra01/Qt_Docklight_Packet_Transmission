#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
    packetsToSend(0), packetsSent(0)
{
    ui->setupUi(this);

    // Populate COM ports
    for (const QSerialPortInfo &info : QSerialPortInfo::availablePorts())
        ui->comboPort->addItem(info.portName());

    // Baud rate options
    ui->comboBaud->addItems({"9600", "19200", "38400", "57600", "115200"});

    ui->spinPackets->setRange(1, 10000);
    ui->spinDelay->setRange(10, 10000);
    ui->btnStop->setEnabled(false);

    connect(&sendTimer, &QTimer::timeout, this, &MainWindow::sendPacket);

    // Prepare log file
    logFile.setFileName("log.txt");
    if (!logFile.open(QIODevice::Append | QIODevice::Text))
        QMessageBox::warning(this, "Error", "Cannot open log.txt for writing!");
    logStream.setDevice(&logFile);
}

MainWindow::~MainWindow()
{
    if (serial.isOpen())
        serial.close();
    logFile.close();
    delete ui;
}

void MainWindow::on_btnStart_clicked()
{
    QString portName = ui->comboPort->currentText();
    if (portName.isEmpty()) {
        QMessageBox::warning(this, "Error", "Select a COM port first!");
        return;
    }

    serial.setPortName(portName);
    serial.setBaudRate(ui->comboBaud->currentText().toInt());
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);

    if (!serial.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, "Error", "Failed to open selected COM port!");
        return;
    }

    packetsToSend = ui->spinPackets->value();
    packetsSent = 0;

    ui->btnStart->setEnabled(false);
    ui->btnStop->setEnabled(true);
    ui->textLog->append("=== Transmission Started ===");

    int delayMs = ui->spinDelay->value();
    sendTimer.start(delayMs);
}

void MainWindow::on_btnStop_clicked()
{
    sendTimer.stop();
    if (serial.isOpen())
        serial.close();

    ui->btnStart->setEnabled(true);
    ui->btnStop->setEnabled(false);
    ui->textLog->append("=== Transmission Stopped ===");
}

void MainWindow::sendPacket()
{
    if (packetsSent >= packetsToSend) {
        on_btnStop_clicked();
        return;
    }

    prepareAndSendPacket();
    packetsSent++;
}

void MainWindow::prepareAndSendPacket()
{
    QByteArray buff(16, 0);
    //unsigned char buff[16];

    buff[0] = '$';
    buff[1] = 'F';
    buff[2] = 'C';

    // Bytes 3–15 = 5, 10, 15, ..., 65
    int value = 5;
    for (int i = 3; i < 16; ++i) {
        buff[i] = static_cast<char>(value);
        value += 5;
    }

    serial.write(buff);

    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
    QString hexStr = buff.toHex(' ').toUpper();
    QString logEntry = QString("[%1] Packet %2: %3")
                           .arg(timestamp)
                           .arg(packetsSent + 1)
                           .arg(hexStr);

    ui->textLog->append(logEntry);
    logStream << logEntry << "\n";
    logStream.flush();
}
