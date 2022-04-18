#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <socket.h>
#include <openglshow.h>
#include "cloudpointthread.h"
#include  <localStruct.h>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:

    void on_connectMID40_clicked();
    void receiveDeviceMSGFromSocket(DEVICEMSG);
    void receiveHandbool(bool);
private slots:
    void on_startSample_clicked();
    void on_pushButton_clicked();


public:
    Ui::MainWindow *ui;
    socket_M *UDP_MID40;
    crc *CRC;
    openglShow *GL;
    cloudPointThread*CPT;
protected:
    void wheelEvent(QWheelEvent *event);
};
#endif // MAINWINDOW_H
