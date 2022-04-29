#ifndef SINS_H
#define SINS_H
#include <QOpenGLWidget>
#include <QObject>
#include <Eigen/Core>
#include <Eigen/src/Core/DenseBase.h>
#include <Eigen/Geometry>
#include <iostream>
struct P_V
{
    float position;
    float velocity;

};

using namespace Eigen;

Q_DECLARE_METATYPE(P_V)
class SINS: public QOpenGLWidget
{
    Q_OBJECT

public:

   SINS();
   float countSpeed(float Vr,float A,float counttime);
   QList< float> countLocation(float x,float y, float angle,float distance);
   Vector3d  Quaternion_Rotation(Vector3d,float pitch,float yaw,float roll);
   float Get_Distance(float ori_latitude,float ori_longtitude,float latitude,float longtitude, float R);
   P_V p_v_KalmanFilter(float Position, float Velocity,float acc,float interval);

};

#endif // SINS_H
