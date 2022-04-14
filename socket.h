#ifndef SOCKET_H
#define SOCKET_H

#include <QObject>
#include <QByteArray>
#include <QCoreApplication>
#include <QHostAddress>
#include <QUdpSocket>
#include <iostream>
#include <qdebug.h>
#include <crc.h>
#include <QMetaType>
#include <QTimer>
class socket_M: public QObject
{
    Q_OBJECT

public:
    socket_M();
    void connectMID_40();
    void needData();    
    void sendHandPackage();

public slots:
    void receive();
    void receiveDeviceMSG(DEVICEMSG);
    void sendHeartPackage();
    void receiveHandbool(bool);
    void receivesendNeedHand(bool);

signals:

    void sendDevicdMSG2Main(DEVICEMSG);
    void sendHandbool2M(bool);

private:
    QUdpSocket *uSocket;
    crc*SOC_CRC;
    QTimer *heartTimer;
};

#endif // SOCKET_H
