#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<DEVICEMSG>( "DEVICEMSG" );
    qRegisterMetaType<cloudData>( "cloudData" );
    qRegisterMetaType<cloudData2show>( "cloudData2show" );
    qRegisterMetaType<cloudData2show>( "iNSData" );
    MainWindow w;
    w.show();



    return a.exec();
}
