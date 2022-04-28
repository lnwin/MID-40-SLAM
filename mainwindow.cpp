#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    CRC=new crc();
    CRC->getUI(*ui);
    UDP_MID40=new socket_M();
    UDP_MID40->moveToThread(&udpThread);
    GL=new openglShow;
    CPT =new cloudPointThread;
    connect(UDP_MID40,SIGNAL(sendDevicdMSG2Main(DEVICEMSG)),this,SLOT(receiveDeviceMSGFromSocket(DEVICEMSG)));
    connect(UDP_MID40,SIGNAL(sendHandbool2M(bool)),this,SLOT(receiveHandbool(bool)));
    connect(UDP_MID40,SIGNAL(sendData2CRC(QByteArray)),CRC,SLOT(receiveData(QByteArray)));
    connect(UDP_MID40,SIGNAL(sendData2CP(QByteArray)),CPT,SLOT(reveiveCPFromSOCKET(QByteArray)));

    connect(CRC,SIGNAL(sendDeviceMSG(DEVICEMSG)),UDP_MID40,SLOT(receiveDeviceMSG(DEVICEMSG)));
    connect(CRC,SIGNAL(sendNeedHand(bool)),UDP_MID40,SLOT(receivesendNeedHand(bool)));
    connect(CRC,SIGNAL(sendHandbool(bool)),UDP_MID40,SLOT(receiveHandbool(bool)));
    connect(CPT,SIGNAL(sendCloudData2GL(cloudData)),GL,SLOT(receivePointCloud(cloudData)));

   // connect(ui->openGLWidget,SIGNAL(wheel2Update()),this,SLOT(updateNow()));
    connect(&udpThread, &QThread::started, UDP_MID40, &socket_M::onInitData);
  //  connect(&udpThread, &QThread::finished, UDP_MID40, &socket_M::deleteLater);
    udpThread.start();

}

MainWindow::~MainWindow()
{
    udpThread.quit();
    udpThread.wait();
    delete ui;
}

void MainWindow::on_connectMID40_clicked()
{
    UDP_MID40->sendHandPackage();
}
void MainWindow::receiveDeviceMSGFromSocket(DEVICEMSG DS)
{

   ui->Device_type->setText("Device_ID:"+DS.TYPE);
   ui->Device_ID->setText("Device_Type:"+QByteArray::fromHex(DS.ID));
   ui->connectMID40->setEnabled(true);
   //UDP_MID40->sendHandPackage();

};
void MainWindow::receiveHandbool(bool ok)
{
    if(ok)
    {
        ui->online_state->setPixmap(QPixmap(":/new/prefix1/picture/on.png"));
        ui->startSample->setEnabled(true);
    }
    else
    {

    }

}

void MainWindow::on_startSample_clicked()
{


    UDP_MID40->needData();


}
float Hex3Dec(QString hex)
{
      bool ok;
      float finaldata;
      long long a =hex.toLongLong(&ok,16);
      QString bin =QString::number(a,2);
      int datalength =bin.length();
     // qDebug()<<bin.length();
    //  qDebug()<<bin;

          if(datalength==32)
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

             finaldata = (double)(-(bin.toInt(&ok,2)+1));
             return finaldata;

          }



         else
       {
             QString data =hex;
             finaldata =(double)data.toLongLong(&ok,16);
             return finaldata;

       }


};
void MainWindow::on_pushButton_clicked()
{

//    QByteArray test;
//    test[0]=0x00;//起始字节，固定为 0xAA
//    test[1]=0x00;//协议版本, 当前为1
//    test[2]=0x08;//数据帧长度,
//    test[3]=0xfe;//数据帧长度,
//    bool ok;
//    qDebug()<<Hex3Dec(test.toHex());
//    qDebug()<<"self"<<test.toHex().toLongLong(&ok,16);
    GL->savecloud();

}

void MainWindow::wheelEvent(QWheelEvent *event)
{

}

