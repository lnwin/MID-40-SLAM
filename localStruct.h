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

    float x[2000];
    float y[2000];
    float z[2000];
    float reflect[2000];


};
Q_DECLARE_METATYPE(cloudData);
struct cloudData2show
{

    float x[2000];
    float y[2000];
    float z[2000];
    float reflect[2000];


};
Q_DECLARE_METATYPE(cloudData2show);
struct iNSData
{
    float Ax_origin;
    float Ay_origin;
    float Az_origin;
    float A_move;

    float V_move;
    float V_down;
    float Angle_pitch;
    float Angle_roll;
    float Angle_yaw;

};

Q_DECLARE_METATYPE(iNSData)
#endif // LOCALSTRUCT_H
