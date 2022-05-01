#ifndef CLOUDPOINTTHREAD_H
#define CLOUDPOINTTHREAD_H

#include <QObject>
#include <QThread>
#include <localStruct.h>
#include <mutex>
#include <qdebug.h>
#include <QList>
#include <QMetaType>
#include <QTime>
#include <QCoreApplication>
#include <sins.h>
class cloudPointThread : public QThread
{
    Q_OBJECT
public:
    cloudPointThread();

signals:
     void sendCloudData2GL(cloudData);
public slots:

    void reveiveCPFromSOCKET(QByteArray);
    void receiveINS(float yaw,float pitch,float roll);
private:
    void run() override;
    float Hex3Dec(QString hex);
    void Delay_MSec(unsigned int msec);
    SINS *clpsins;
};

#endif // CLOUDPOINTTHREAD_H
