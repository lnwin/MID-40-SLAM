#include "mainwindow.h"
#include "ui_mainwindow.h"
bool canSample=true;
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
    CPT->moveToThread(&cldThread); //  cldThread
    insPort = new INSport();
    insPort->moveToThread(&insThread);

    connect(UDP_MID40,SIGNAL(sendDevicdMSG2Main(DEVICEMSG)),this,SLOT(receiveDeviceMSGFromSocket(DEVICEMSG)));
    connect(UDP_MID40,SIGNAL(sendHandbool2M(bool)),this,SLOT(receiveHandbool(bool)));
    connect(UDP_MID40,SIGNAL(sendbool2M(bool)),this,SLOT(receivebool2M(bool)));
    connect(UDP_MID40,SIGNAL(sendData2CRC(QByteArray)),CRC,SLOT(receiveData(QByteArray)));
    connect(UDP_MID40,SIGNAL(sendData2CP(QByteArray)),CPT,SLOT(reveiveCPFromSOCKET(QByteArray)));

    connect(CRC,SIGNAL(sendDeviceMSG(DEVICEMSG)),UDP_MID40,SLOT(receiveDeviceMSG(DEVICEMSG)));
    connect(CRC,SIGNAL(sendNeedHand(bool)),UDP_MID40,SLOT(receivesendNeedHand(bool)));
    connect(CRC,SIGNAL(sendHandbool(bool)),UDP_MID40,SLOT(receiveHandbool(bool)));
    connect(CPT,SIGNAL(sendCloudData2GL(cloudData)),GL,SLOT(receivePointCloud(cloudData)));

    connect(this,SIGNAL(sendINSport(QString)),insPort,SLOT(receiveINSPort(QString)));

   // connect(ui->openGLWidget,SIGNAL(wheel2Update()),this,SLOT(updateNow()));
    connect(&udpThread, &QThread::started, UDP_MID40, &socket_M::onInitData);
    connect(&insThread, &QThread::started, insPort, &INSport::onInit);
  //  connect(&udpThread, &QThread::finished, UDP_MID40, &socket_M::deleteLater);
    udpThread.start();
    cldThread.start();
    insThread.start();
    searchPort();

}

MainWindow::~MainWindow()
{
    udpThread.quit();
    udpThread.wait();
    cldThread.quit();
    cldThread.wait();
    insThread.quit();
    insThread.wait();
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
void MainWindow::receivebool2M(bool received)
{

  if(received)
  {
     ui->startSample->setText("Stop Sample");
     canSample=false;
  }

}
void MainWindow::on_startSample_clicked()
{

  if(canSample)

  {
     UDP_MID40->needData();
     ui->startSample->setText("Stop Sample");
     canSample=false;
  }

  else
  {
     canSample=true;
     ui->startSample->setText("Start Sample");
     UDP_MID40->stopData();

  }


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
  //  GL->savecloud();

    SINS *Csins ;
    glm::vec3 cameraPos = glm::vec3(1.0f, 0.0f, 1.0f);

    Vector3d CAM(100,0,0);
//    CAM.x()= cameraPos.x;
//    CAM.y()= cameraPos.y;
//    CAM.z()= cameraPos.z;

    Vector3d AK = Csins->Quaternion_Rotation(CAM,0,45,0);


    qDebug()<<"AS-x"<<AK.x();
    qDebug()<<"AS-y"<<AK.y();
    qDebug()<<"AS-z"<<AK.z();
   // qDebug()<<"cameraPos.y"<<cameraPos.y;
   // qDebug()<<"cameraPos.z"<<cameraPos.z;

}
void MainWindow::on_INSButton_clicked()
{

    emit sendINSport(ui->INScomboBox->currentText());
}
void MainWindow::searchPort()
{
     ui->INScomboBox ->clear();
     ui->GNSScomboBox->clear();
    foreach (const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {

            ui->INScomboBox->addItem(serial.portName());
            ui->INScomboBox->setCurrentIndex(ui->INScomboBox->count()-1);
            ui->GNSScomboBox->addItem(serial.portName());
            ui->GNSScomboBox->setCurrentIndex(ui->GNSScomboBox->count()-2);
            serial.close();

        }
    }
}
