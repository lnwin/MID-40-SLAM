#include "insport.h"

INSport::INSport(QObject *parent) : QObject(parent)
{

}

void INSport::onInit()
{

    INS=new QSerialPort;

}

void INSport::receiveINSPort(QString portN)
{

    INS->setPortName(portN);//设置串口名
    INS->open(QIODevice::ReadWrite);//以读写方式打开串口
    INS->setBaudRate(QSerialPort::Baud115200);//波特率
    INS->setDataBits(QSerialPort::Data8);//数据位
    INS->setParity(QSerialPort::NoParity);//校验位
    INS->setStopBits(QSerialPort::OneStop);//停止位
}
