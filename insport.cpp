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
    connect(INS,SIGNAL(readyRead()),this,SLOT(readData()));
    QByteArray startMeg;
    startMeg[0]=0xa5;
    startMeg[1]=0x5a;
    startMeg[2]=0x04;
    startMeg[3]=0x01;
    startMeg[4]=0x05;
    startMeg[5]=0xaa;
    INS->write(startMeg);
     qDebug()<<"port opend";
}
void INSport::readData()
{
    float yaw,roll,pitch;//,a_x_ori,a_y_ori,a_z_ori;

    iNSData insdata;
    QByteArray buf;
    buf = INS->readAll();
    if(!buf.isEmpty()&&buf.length()>30)
    {



        yaw =Hex2Dec_yrp(buf.mid(3,2))*0.1;
        pitch =Hex2Dec_yrp(buf.mid(5,2))*0.1;
        roll = Hex2Dec_yrp(buf.mid(7,2))*0.1;


        emit sendINS2CLP(yaw,pitch,roll);
//        a_x_ori=Hex2Dec_yrp(buf.mid(9,2))*G/16384;
//        a_y_ori=Hex2Dec_yrp(buf.mid(11,2))/16384*G;//前进方向
//        a_z_ori=Hex2Dec_yrp(buf.mid(13,2))*G/16384;//垂直方向
//        a_y= (a_y_ori)-sin((pitch*PI)/180)*G;

//        V=V+a_y*0.02;
//        distance = V*0.02+0.5*a_y*0.02*0.02;
//        //qDebug()<<distance<<"  "<<V;
//        xt=xt+cos(yaw*PI/180)*distance;
//        yt=yt+sin(yaw*PI/180)*distance;
//        PXY.x=xt*10;
//        PXY.y =yt*10;
//        emit sendXY2GL(PXY);

        //qDebug()<<"yaw"<<yaw;
//        qDebug()<<"pitch"<<pitch;
//        qDebug()<<"roll"<<roll;
//        insdata.Angle_yaw = yaw;
//        insdata.Angle_roll =roll;
//        insdata.Angle_pitch =pitch;
//        insdata.Ax_origin =a_x_ori;
//        insdata.Ay_origin =a_y_ori;
//        insdata.Az_origin =a_z_ori;
//        insdata.A_move =a_y;
//        insdata.V_move =V;
//        emit send2Main(insdata);

        //计算
  }
}
float INSport::Hex2Dec_yrp(QByteArray hex)
 {
    bool ok;
    float finaldata;
//    QByteArray zz;
//    zz.append(hex.at(1));
//    zz.append(hex.at(0));
//    int a = zz.toHex().toInt(&ok,16);
 //   int b = zz.mid(0,1).toHex().toInt(&ok,16);
    int a = hex.toHex().toInt(&ok,16);
    int b = hex.mid(0,1).toHex().toInt(&ok,16);
    QString bin =QString::number(a,2);
    int datalength =bin.length();
    if(b>=128)
 {

           for (int i=0;i<datalength;i++)
           {
               if(bin[i]=="0")
               {
                   bin[i]='1';
               }
               else
               {
                   bin[i]='0';
               }

           }

           finaldata = (float)(-(bin.toInt(&ok,2)+1));
           return finaldata;

     }
    else
    {
          finaldata = (float)((bin.toInt(&ok,2)));
          return finaldata;
    }
 };
