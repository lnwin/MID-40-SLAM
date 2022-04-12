#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    CRC=new crc();
    UDP_MID40=new socket();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_connectMID40_clicked()
{
     UDP_MID40->connectMID_40();
}
