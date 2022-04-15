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
    GL=new openglShow;
    connect(UDP_MID40,SIGNAL(sendDevicdMSG2Main(DEVICEMSG)),this,SLOT(receiveDeviceMSGFromSocket(DEVICEMSG)));
    connect(UDP_MID40,SIGNAL(sendHandbool2M(bool)),this,SLOT(receiveHandbool(bool)));
    connect(UDP_MID40,SIGNAL(sendData2CRC(QByteArray)),CRC,SLOT(receiveData(QByteArray)));

    connect(CRC,SIGNAL(sendDeviceMSG(DEVICEMSG)),UDP_MID40,SLOT(receiveDeviceMSG(DEVICEMSG)));
    connect(CRC,SIGNAL(sendNeedHand(bool)),UDP_MID40,SLOT(receivesendNeedHand(bool)));
    connect(CRC,SIGNAL(sendHandbool(bool)),UDP_MID40,SLOT(receiveHandbool(bool)));
    connect(CRC,SIGNAL(sendCloudData2GL(cloudData)),GL,SLOT(receivePointCloud(cloudData)));

}

MainWindow::~MainWindow()
{
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

void MainWindow::on_pushButton_clicked()
{
    QByteArray data;
    CRC->analysisPointCloud(data);
}
