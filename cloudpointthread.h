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
class cloudPointThread : public QThread
{
    Q_OBJECT
public:
    cloudPointThread();

signals:
     void sendCloudData2GL(cloudData);
public slots:

    void reveiveCPFromSOCKET(QByteArray);

private:
    void run() override;
    float Hex3Dec(QString hex);
    void Delay_MSec(unsigned int msec);
};

#endif // CLOUDPOINTTHREAD_H
