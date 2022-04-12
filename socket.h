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
class socket: public QObject
{
    Q_OBJECT

public:
    socket();
    void connectMID_40();
    void needData();
    void sendHeartBag();

public slots:
    void receive();
private:
    QUdpSocket *uSocket;
    crc*SOC_CRC;
};

#endif // SOCKET_H
