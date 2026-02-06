#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <QFile>
#include <QTextStream>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnStart_clicked();
    void on_btnStop_clicked();
    void sendPacket();

private:
    Ui::MainWindow *ui;
    QSerialPort serial;
    QTimer sendTimer;
    QFile logFile;
    QTextStream logStream;

    int packetsToSend;
    int packetsSent;
    void
    prepareAndSendPacket();
};

#endif // MAINWINDOW_H
