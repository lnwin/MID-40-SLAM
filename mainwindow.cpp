#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    CRC=new crc();
    UDP_MID40=new socket_M();
    connect(UDP_MID40,SIGNAL(sendDevicdMSG2Main(DEVICEMSG)),this,SLOT(receiveDeviceMSGFromSocket(DEVICEMSG)));
    connect(UDP_MID40,SIGNAL(sendHandbool2M(bool)),this,SLOT(receiveHandbool(bool)));

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
