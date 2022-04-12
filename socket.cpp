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
       if(SOC_CRC->checkCRC(ba))
       {
           qDebug()<<"socket check passed";
       }
    }
}
void socket::sendHeartBag()
{

};
void socket::connectMID_40()
{
        QByteArray msg;
        QByteArray data;

        data[0]=0x00;
        data[1]=0x01;
        data[2]=0xcd;//����IP��ַ
        data[3]=0xa8;//����IP��ַ
        data[4]=0x01;//����IP��ַ
        data[5]=0x4d;//����IP��ַ
        data[6]=0xd6;//������������UDPĿ�Ķ˿�
        data[7]=0xd8;//������������UDPĿ�Ķ˿�
        data[8]=0xd6;//��������ָ��UDPĿ�Ķ˿�
        data[9]=0xd8;//��������ָ��UDPĿ�Ķ˿�
        data[10]=0xd6;//��������IMU UDPĿ�Ķ˿�
        data[11]=0xd8;//��������IMU UDPĿ�Ķ˿�
        bool ok;
        QByteArray head;
        head[0]=0xaa;//��ʼ�ֽڣ��̶�Ϊ 0xAA
        head[1]=0x01;//Э��汾, ��ǰΪ1
        head[2]=0x19;//����֡����,
        head[3]=0x00;//����֡����,
        head[4]=0x00;//��������
        head[5]=0x11;//����֡���к�
        head[6]=0x00;//����֡���к�
        uint8_t MESG_head[7];
        for(int i=0;i<7;i++)
        {
            MESG_head[i]=(uint8_t)head.at(i);
        }
        QByteArray crc16;
       // crc16=QByteArray::number(SOC_CRC->FastCRC16(MESG_head,7)).toHex().toInt(&ok,16);

       // qDebug()<<QByteArray::number(SOC_CRC->FastCRC16(MESG_head,7), 16);
       // msg[0]=QByteArray::number(SOC_CRC->FastCRC16(MESG_head,7),16).at(2);

        qDebug()<<"crc16"<<QByteArray::number(SOC_CRC->FastCRC16(MESG_head,7), 16);
        msg.append(head);
        msg.append(crc16.data());
        msg.append(data);

        uint8_t MESG_all[21];

        for(int i=0;i<21;i++)
        {
            MESG_all[i]=(uint8_t)msg.at(i);
        }
        QByteArray crc32;
        crc32=QByteArray::number(SOC_CRC->FastCRC32(MESG_all,21),16);
        qDebug()<<"crc32"<<crc32.data();
        msg.append(crc32.data());
        //msg = QByteArray::fromHex(cmd.toLatin1());//����ͨ��
        uSocket->writeDatagram(msg, QHostAddress("192.168.1.20"), 65000);

        qDebug()<<"msg.toHex()"<<msg.toHex();

}
