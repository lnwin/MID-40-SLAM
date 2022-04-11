#include "socket.h"

socket::socket()
{
        SOC_CRC=new crc;
        uSocket = new QUdpSocket;
        uSocket->bind(QHostAddress("192.168.1.77"), 55000);
        connect(uSocket, SIGNAL(readyRead()), this, SLOT(receive()));
        qDebug()<<"UDP success";

}
void socket::receive()
{
    QByteArray ba;
    while(uSocket->hasPendingDatagrams())
    {
        ba.resize(uSocket->pendingDatagramSize());
        uSocket->readDatagram(ba.data(), ba.size());

        qDebug() << ba.toHex()+"\n";
        qDebug() << ba.remove(7,27).toHex()+"\n";
        uint16_t data =ba.toUShort();

        uint8_t sk[4]= {4,"c",4,9};

        qDebug() << QString::number(SOC_CRC->FastCRC16(data,sk,7), 16);
                  //  QString HEXNum = QString("%1").arg(setNum,0,16,QLatin1Char('0')).toUtf8();;
    }
}
