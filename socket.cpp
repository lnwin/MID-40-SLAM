#include "socket.h"

socket::socket()
{
        SOC_CRC = new crc;
        uSocket = new QUdpSocket;
        heartTimer =new QTimer;
        uSocket->bind(QHostAddress("192.168.1.77"), 55000);
        heartTimer->setInterval(1000);

        connect(uSocket, SIGNAL(readyRead()), this, SLOT(receive()));
        connect(SOC_CRC,SIGNAL(sendDeviceMSG(DEVICEMSG)),this,SLOT(receiveDeviceMSG(DEVICEMSG)));
        connect(SOC_CRC,SIGNAL(sendHandbool(bool)),this,SLOT(receiveHandbool(bool)));
        connect(heartTimer, SIGNAL(timeout()), this, SLOT(sendHeartPackage()));

        qDebug()<<"UDP success";

}
void socket::receive()
{
    QByteArray ba;
    while(uSocket->hasPendingDatagrams())
    {
        ba.resize(uSocket->pendingDatagramSize());
        uSocket->readDatagram(ba.data(), ba.size());

       if(!ba.isNull())
       {
           if(SOC_CRC->checkCRC(ba))
           {
               //qDebug()<<"socket check passed";
           }
       }

    }
}
void socket::receiveDeviceMSG(DEVICEMSG DM)
{

   if(DM.TYPE=="01")
   {
       DM.TYPE="MID_40";
   }
   emit sendDevicdMSG2Main(DM);

};
void socket::receiveHandbool(bool handed)
{
   if(handed)
   {
       qDebug()<<"start hand";
      heartTimer->start();
   }
   else
   {

   }
}
void socket::sendHeartPackage()
{


    QByteArray msg;
    QByteArray data;
    data[0]=0x00;
    data[1]=0x03;


    QByteArray head;
    head[0]=0xaa;//起始字节，固定为 0xAA
    head[1]=0x01;//协议版本, 当前为1
    head[2]=0x0f;//数据帧长度,
    head[3]=0x00;//数据帧长度,
    head[4]=0x00;//命令类型
    head[5]=0x00;//数据帧序列号
    head[6]=0x00;//数据帧序列号
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
    //msg = QByteArray::fromHex(cmd.toLatin1());//测试通过
    uSocket->writeDatagram(msg, QHostAddress("192.168.1.20"), 65000);


};
void socket::sendHandPackage()
{
    QByteArray msg;
    QByteArray data;
    data[0]=0x00;
    data[1]=0x01;
    data[2]=0xc0;//主机IP地址
    data[3]=0xa8;//主机IP地址
    data[4]=0x01;//主机IP地址
    data[5]=0x4d;//主机IP地址
    data[6]=0xd8;//主机点云数据UDP目的端口
    data[7]=0xd6;//主机点云数据UDP目的端口
    data[8]=0xd8;//主机控制指令UDP目的端口
    data[9]=0xd6;//主机控制指令UDP目的端口
    data[10]=0xd8;//主机控制IMU UDP目的端口
    data[11]=0xd6;//主机控制IMU UDP目的端口

    QByteArray head;
    head[0]=0xaa;//起始字节，固定为 0xAA
    head[1]=0x01;//协议版本, 当前为1
    head[2]=0x19;//数据帧长度,
    head[3]=0x00;//数据帧长度,
    head[4]=0x00;//命令类型
    head[5]=0x00;//数据帧序列号
    head[6]=0x00;//数据帧序列号
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
    //msg = QByteArray::fromHex(cmd.toLatin1());//测试通过
    uSocket->writeDatagram(msg, QHostAddress("192.168.1.20"), 65000);
}
void socket::connectMID_40()
{


       // qDebug()<<"msg.toHex()"<<msg.toHex();

}
