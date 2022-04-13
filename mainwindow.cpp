#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    CRC=new crc();
    UDP_MID40=new socket();
    connect(UDP_MID40,SIGNAL(sendDevicdMSG2Main(DEVICEMSG)),this,SLOT(receiveDeviceMSGFromSocket(DEVICEMSG)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_connectMID40_clicked()
{
     UDP_MID40->connectMID_40();

}
void MainWindow::receiveDeviceMSGFromSocket(DEVICEMSG DS)
{

   ui->Device_type->setText("Device_ID:"+DS.TYPE);
   ui->Device_ID->setText("Device_Type:"+QByteArray::fromHex(DS.ID));
   ui->connectMID40->setEnabled(true);
   UDP_MID40->sendHandPackage();

};
