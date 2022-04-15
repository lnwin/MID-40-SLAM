#include "openglshow.h"

openglShow::openglShow()
{

}
openglShow::~openglShow()
{

}
openglShow::openglShow(QWidget *parent) : QOpenGLWidget(parent)
{
   //connect(SOC_CRC, SIGNAL(sendCloudData2GL(cloudData)), this, SLOT(receive()));

}
void openglShow::receivePointCloud(cloudData da)
{
    qDebug()<<"Point Data";
};
