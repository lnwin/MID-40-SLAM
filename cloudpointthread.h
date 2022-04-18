#ifndef CLOUDPOINTTHREAD_H
#define CLOUDPOINTTHREAD_H

#include <QObject>
#include <QThread>
#include <localStruct.h>
#include <mutex>
class cloudPointThread : public QThread
{
    Q_OBJECT
public:
    cloudPointThread();

signals:
     void sendCloudData2GL(QList<float>,QList<float>,QList<float>,QList<float>);
public slots:

    void reveiveCPFromSOCKET(QByteArray);

private:
    void run() override;
    float Hex3Dec(QString hex);
};

#endif // CLOUDPOINTTHREAD_H
