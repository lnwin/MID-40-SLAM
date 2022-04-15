#ifndef LOCALSTRUCT_H
#define LOCALSTRUCT_H
#include <QMetaType>
struct DEVICEMSG
{
     QByteArray TYPE;
     QByteArray ID;
};

Q_DECLARE_METATYPE(DEVICEMSG);
struct cloudData
{

    float x[100];
    float y[100];
    float z[100];
    float reflect[100];


};
Q_DECLARE_METATYPE(cloudData);
#endif // LOCALSTRUCT_H
