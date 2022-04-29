#include "socket.h"

socket_M::socket_M()
{


}
void socket_M::onInitData()
{
    SOC_CRC = new crc;
    uSocket = new QUdpSocket;
    uSocketCloudP = new QUdpSocket;
    heartTimer =new QTimer;
  // GL =new openglShow;
    uSocket->bind(QHostAddress("192.168.1.77"), 55000);
    uSocketCloudP->bind(QHostAddress("192.168.1.77"), 55100);
    heartTimer->setInterval(1000);
    //uSocket->moveToThread(&socthread);
    connect(uSocket, SIGNAL(readyRead()), this, SLOT(receive()));
    connect(uSocketCloudP, SIGNAL(readyRead()), this, SLOT(readCloudP()));
    connect(heartTimer, SIGNAL(timeout()), this, SLOT(sendHeartPackage()));
   // socthread.start();
    qDebug()<<"UDP success";
}
void socket_M::receive()
{
    QByteArray ba;
    while(uSocket->hasPendingDatagrams())
    {
        ba.resize(uSocket->pendingDatagramSize());
        uSocket->readDatagram(ba.data(), ba.size());

       if(!ba.isNull())
       {

           emit sendData2CRC(ba);
//           if(SOC_CRC->checkCRC(ba))
//           {
            //  qDebug()<<"socket check passed";
//           }
       }

    }
}

void socket_M::readCloudP()
{

    QByteArray ba;
    while(uSocketCloudP->hasPendingDatagrams())
    {
        ba.resize(uSocketCloudP->pendingDatagramSize());
        uSocketCloudP->readDatagram(ba.data(), ba.size());
        if(!ba.isNull())
        {
             QString HEAD =ba.mid(0,1).toHex();
            if(HEAD=="05")
            {
                //qDebug()<<";";
                emit sendData2CP(ba);

            }

        }
    }
}
void socket_M::receiveDeviceMSG(DEVICEMSG DM)
{

   if(DM.TYPE=="01")
   {
       DM.TYPE="MID_40";
   }
   emit sendDevicdMSG2Main(DM);

};
void socket_M::receivesendNeedHand(bool need)
{
    if(need)
    {
        heartTimer->start();
    }
    else
    {

    }
};
void socket_M::receiveHandbool(bool handed)
{
   if(handed)
   {
      qDebug()<<"hand success";
      emit sendHandbool2M(true);
   }
   else
   {
      qDebug()<<"hand false";
      emit sendHandbool2M(false);
   }
}
void socket_M::sendHeartPackage()
{


    QByteArray msg;
    QByteArray data;
    data[0]=0x00;
    data[1]=0x03;


    QByteArray head;
    head[0]=0xaa;//��ʼ�ֽڣ��̶�Ϊ 0xAA
    head[1]=0x01;//Э��汾, ��ǰΪ1
    head[2]=0x0f;//����֡����,
    head[3]=0x00;//����֡����,
    head[4]=0x00;//��������
    head[5]=0x00;//����֡���к�
    head[6]=0x00;//����֡���к�
    uint8_t MESG_head[7];
    for(int i=0;i<7;i++)
    {
        MESG_head[i]=(uint8_t)head.at(i);
    }
    QByteArray crc16;
    crc16=QByteArray::fromHex(QByteArray::number(SOC_CRC->FastCRC16(MESG_head,7), 16));

   // qDebug()<<QByteArray::number(SOC_CRC->FastCRC16(MESG_head,7), 16);
   // msg[0]=QByteArray::number(SOC_CRC->FastCRC16(MESG_head,7),16).at(2);

   // qDebug()<<"crc16"<<crc16;
    msg.append(head);
    msg.append(crc16.at(1));
    msg.append(crc16.at(0));
    msg.append(data);

    uint8_t MESG_all[11];

    for(int i=0;i<11;i++)
    {
        MESG_all[i]=(uint8_t)msg.at(i);
    }
    QByteArray crc32;
    crc32=QByteArray::fromHex(QByteArray::number(SOC_CRC->FastCRC32(MESG_all,11),16));
   // qDebug()<<"crc32"<<crc32;
    msg.append(crc32.at(3));
    msg.append(crc32.at(2));
    msg.append(crc32.at(1));
    msg.append(crc32.at(0));
    //msg = QByteArray::fromHex(cmd.toLatin1());//����ͨ��
    uSocket->writeDatagram(msg, QHostAddress("192.168.1.20"), 65000);


};
void socket_M::sendHandPackage()
{
    QByteArray msg;
    QByteArray data;
    data[0]=0x00;
    data[1]=0x01;
    data[2]=0xc0;//����IP��ַ
    data[3]=0xa8;//����IP��ַ
    data[4]=0x01;//����IP��ַ
    data[5]=0x4d;//����IP��ַ
    data[6]=0x3c;//������������UDPĿ�Ķ˿�
    data[7]=0xd7;//������������UDPĿ�Ķ˿�
    data[8]=0xd8;//��������ָ��UDPĿ�Ķ˿�
    data[9]=0xd6;//��������ָ��UDPĿ�Ķ˿�
    data[10]=0xd8;//��������IMU UDPĿ�Ķ˿�
    data[11]=0xd6;//��������IMU UDPĿ�Ķ˿�

    QByteArray head;
    head[0]=0xaa;//��ʼ�ֽڣ��̶�Ϊ 0xAA
    head[1]=0x01;//Э��汾, ��ǰΪ1
    head[2]=0x19;//����֡����,
    head[3]=0x00;//����֡����,
    head[4]=0x00;//��������
    head[5]=0x00;//����֡���к�
    head[6]=0x00;//����֡���к�
    uint8_t MESG_head[7];
    for(int i=0;i<7;i++)
    {
        MESG_head[i]=(uint8_t)head.at(i);
    }
    QByteArray crc16;
    crc16=QByteArray::fromHex(QByteArray::number(SOC_CRC->FastCRC16(MESG_head,7), 16));

   // qDebug()<<QByteArray::number(SOC_CRC->FastCRC16(MESG_head,7), 16);
   // msg[0]=QByteArray::number(SOC_CRC->FastCRC16(MESG_head,7),16).at(2);

   // qDebug()<<"crc16"<<crc16;
    msg.append(head);
    msg.append(crc16.at(1));
    msg.append(crc16.at(0));
    msg.append(data);

    uint8_t MESG_all[21];

    for(int i=0;i<21;i++)
    {
        MESG_all[i]=(uint8_t)msg.at(i);
    }
    QByteArray crc32;
    crc32=QByteArray::fromHex(QByteArray::number(SOC_CRC->FastCRC32(MESG_all,21),16));
   // qDebug()<<"crc32"<<crc32;
    msg.append(crc32.at(3));
    msg.append(crc32.at(2));
    msg.append(crc32.at(1));
    msg.append(crc32.at(0));
    //msg = QByteArray::fromHex(cmd.toLatin1());//����ͨ��
    uSocket->writeDatagram(msg, QHostAddress("192.168.1.20"), 65000);
}
void socket_M::connectMID_40()
{

    QByteArray head;
    head[0]=0xaa;//��ʼ�ֽڣ��̶�Ϊ 0xAA
    head[1]=0x01;//Э��汾, ��ǰΪ1
    head[2]=0x19;//����֡����,
    head[3]=0x00;//����֡����,
    head[4]=0x00;//��������
    head[5]=0x00;//����֡���к�
    head[6]=0x00;//����֡���к�
    uint8_t MESG_head[7];
    QByteArray sk;
    sk.append(head[3]);
    sk.append(head[2]);
   // qDebug()<< sk.toHex().toInt(0,16); //����ͨ��

}
void socket_M::needData()
{
    QByteArray msg;
    QByteArray data;
    data[0]=0x00;
    data[1]=0x04;
    data[2]=0x01;//��ʼ����


    QByteArray head;
    head[0]=0xaa;//��ʼ�ֽڣ��̶�Ϊ 0xAA
    head[1]=0x01;//Э��汾, ��ǰΪ1
    head[2]=0x10;//����֡����,
    head[3]=0x00;//����֡����,
    head[4]=0x00;//��������
    head[5]=0x00;//����֡���к�
    head[6]=0x00;//����֡���к�
    uint8_t MESG_head[7];
    for(int i=0;i<7;i++)
    {
        MESG_head[i]=(uint8_t)head.at(i);
    }
    QByteArray crc16;
    crc16=QByteArray::fromHex(QByteArray::number(SOC_CRC->FastCRC16(MESG_head,7), 16));

   // qDebug()<<QByteArray::number(SOC_CRC->FastCRC16(MESG_head,7), 16);
   // msg[0]=QByteArray::number(SOC_CRC->FastCRC16(MESG_head,7),16).at(2);

   // qDebug()<<"crc16"<<crc16;
    msg.append(head);
    msg.append(crc16.at(1));
    msg.append(crc16.at(0));
    msg.append(data);
    uint8_t MESG_all[12];
    for(int i=0;i<12;i++)
    {
        MESG_all[i]=(uint8_t)msg.at(i);
    }
    QByteArray crc32;
    crc32=QByteArray::fromHex(QByteArray::number(SOC_CRC->FastCRC32(MESG_all,12),16));
   // qDebug()<<"crc32"<<crc32;
    msg.append(crc32.at(3));
    msg.append(crc32.at(2));
    msg.append(crc32.at(1));
    msg.append(crc32.at(0));
    //msg = QByteArray::fromHex(cmd.toLatin1());//����ͨ��
    uSocket->writeDatagram(msg, QHostAddress("192.168.1.20"), 65000);


}
void socket_M::stopData()
{
    QByteArray msg;
    QByteArray data;
    data[0]=0x00;
    data[1]=0x04;
    data[2]=0x00;//ֹͣ����


    QByteArray head;
    head[0]=0xaa;//��ʼ�ֽڣ��̶�Ϊ 0xAA
    head[1]=0x01;//Э��汾, ��ǰΪ1
    head[2]=0x10;//����֡����,
    head[3]=0x00;//����֡����,
    head[4]=0x00;//��������
    head[5]=0x00;//����֡���к�
    head[6]=0x00;//����֡���к�
    uint8_t MESG_head[7];
    for(int i=0;i<7;i++)
    {
        MESG_head[i]=(uint8_t)head.at(i);
    }
    QByteArray crc16;
    crc16=QByteArray::fromHex(QByteArray::number(SOC_CRC->FastCRC16(MESG_head,7), 16));

   // qDebug()<<QByteArray::number(SOC_CRC->FastCRC16(MESG_head,7), 16);
   // msg[0]=QByteArray::number(SOC_CRC->FastCRC16(MESG_head,7),16).at(2);

   // qDebug()<<"crc16"<<crc16;
    msg.append(head);
    msg.append(crc16.at(1));
    msg.append(crc16.at(0));
    msg.append(data);
    uint8_t MESG_all[12];
    for(int i=0;i<12;i++)
    {
        MESG_all[i]=(uint8_t)msg.at(i);
    }
    QByteArray crc32;
    crc32=QByteArray::fromHex(QByteArray::number(SOC_CRC->FastCRC32(MESG_all,12),16));
   // qDebug()<<"crc32"<<crc32;
    msg.append(crc32.at(3));
    msg.append(crc32.at(2));
    msg.append(crc32.at(1));
    msg.append(crc32.at(0));
    //msg = QByteArray::fromHex(cmd.toLatin1());//����ͨ��
    uSocket->writeDatagram(msg, QHostAddress("192.168.1.20"), 65000);

}
