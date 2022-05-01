#ifndef INSPORT_H
#define INSPORT_H

#include <QObject>
#include <QDebug>
#include <QSerialPort>
#include <localStruct.h>
class INSport : public QObject
{
    Q_OBJECT
public:
    explicit INSport(QObject *parent = nullptr);
    void onInit();
     QSerialPort *INS;
signals:
    void sendINS2CLP(float yaw,float pitch,float roll);
public slots:

    void receiveINSPort(QString);
    void readData();
protected:
     float Hex2Dec_yrp(QByteArray hex);
};

#endif // INSPORT_H
