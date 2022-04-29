#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <socket.h>
#include <openglshow.h>
#include "cloudpointthread.h"
#include  <localStruct.h>
#include <qlist.h>
#include <QMetaType>
#include <QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <insport.h>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void searchPort();

public slots:

    void on_connectMID40_clicked();
    void receiveDeviceMSGFromSocket(DEVICEMSG);
    void receiveHandbool(bool);
    void receivebool2M(bool);
private slots:
    void on_startSample_clicked();
    void on_pushButton_clicked();
    void on_INSButton_clicked();

signals:
     void sendINSport(QString);
public:
    Ui::MainWindow *ui;
    socket_M *UDP_MID40;
    crc *CRC;
    openglShow *GL;
    cloudPointThread*CPT;
    INSport *insPort;
    QThread udpThread;
    QThread cldThread;
    QThread insThread;

protected:

};
#endif // MAINWINDOW_H
