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
        bool ok;
        int MESG_NUMBER=ba.mid(2,1).toHex().toInt(&ok, 16)+ba.mid(3,1).toHex().toInt(&ok, 16)*256;
        int MESG_NUMBER_nCRC=MESG_NUMBER-4;
        qDebug() << ba.toHex()+"\n";
        qDebug()<<MESG_NUMBER;
        uint8_t MESG_head[7];

        for(int i=0;i<7;i++)
        {
            MESG_head[i]=(uint8_t)ba.at(i);
        }

        uint8_t MESG_all[1404];

        for(int i=0;i<MESG_NUMBER_nCRC;i++)
        {
            MESG_all[i]=(uint8_t)ba.at(i);
        }
      //  qDebug()<<QString::number(SOC_CRC->FastCRC16(MESG_head,7), 16);
        QByteArray check_crc16;
        check_crc16.append(ba.at(8));
        check_crc16.append(ba.at(7));
        qDebug()<<check_crc16.toHex().toInt(&ok, 16);
        qDebug()<<QString::number(SOC_CRC->FastCRC16(MESG_head,7), 10);
        qDebug()<<QString::number(SOC_CRC->FastCRC32(MESG_all,MESG_NUMBER_nCRC), 16);

                  //  QString HEXNum = QString("%1").arg(setNum,0,16,QLatin1Char('0')).toUtf8();;
    }
}
