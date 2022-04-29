#ifndef INSPORT_H
#define INSPORT_H

#include <QObject>
#include <QDebug>
#include <QSerialPort>
class INSport : public QObject
{
    Q_OBJECT
public:
    explicit INSport(QObject *parent = nullptr);
    void onInit();
     QSerialPort *INS;
signals:

public slots:

    void receiveINSPort(QString);
};

#endif // INSPORT_H
