#include "sins.h"
#include "QDebug"
//****************************************
//SINS参数

float H[2][2];
float K[2][2];
float P[2][2];
P_V p_v;
QList< float> location;
//****************************************
//四元数
const float PI =3.1415926535898;
using namespace Eigen;
using namespace std;


//****************************************
SINS::SINS()
{
   p_v.position=0;
   p_v.velocity=0;
   K[0][0]=0;K[0][1]=0;K[1][0]=0;K[1][1]=0;
   P[0][0]=0;P[0][1]=0;P[1][0]=0;P[1][1]=0;
}

float SINS::countSpeed(float Vr,float A,float counttime)
{
     float V;
     V= Vr+A*counttime;
     return V;
}
float SINS::Get_Distance(float ori_latitude,float ori_longtitude,float latitude,float longtitude,float R)
{
   float distance;
   float alng = ori_longtitude*PI/180;
   float blat = ori_latitude*PI/180;
   float Alng = longtitude*PI/180;
   float Blat = latitude*PI/180;



   float Dlng = abs(Alng-alng);
   float Dlat = abs(Blat-blat);



   float AD = 2*sin(Dlng/2)*cos(Blat)*R;
   float BC = 2*sin(Dlng/2)*cos(blat)*R;

   float AC = 2*sin(Dlat/2)*R;

   float AB = sqrt(AC*AC +BC*AD);

   float AOB = 2*atan((AB/2)/sqrt(R*R-(AB*AB/4)));


   distance = AOB*R*1000;

   return distance;


}

P_V SINS::p_v_KalmanFilter(float Position, float Velocity,float acc, float interval)
{

   float Qp=1.0;
   float Qv=2.0;
   float R=1;
   float F[2][2];
   F[0][0]=1;
   F[0][1]=interval;
   F[1][0]=0;
   F[1][1]=1;

   float B[1][2];
   B[0][0]=0.5*interval*interval;
   B[0][1]=interval*interval;
//---------------------状态方程
   p_v.position=p_v.position+p_v.velocity*interval+0.5*acc*interval*interval;
   p_v.velocity=p_v.velocity+acc*interval;

  // p_v.position=p_v.position+p_v.velocity*interval+0.5*acc*interval*interval;
  // p_v.velocity=p_v.velocity+acc*interval;
//---------------------误差协方差矩阵
   P[0][0]=P[0][0]+P[1][0]*interval+interval*P[0][1]+Qp;//+interval*interval*P[1][1]//数值过小，省略去
   P[0][1]=P[0][1]+interval*P[1][1];
   P[1][0]=P[1][0]+interval*P[1][1];
   P[1][1]=P[1][1]+Qv;
//---------------------以上预测部分完成

//---------------------更新部分
   K[0][0]=P[0][0]/(P[0][0]+R);
   K[0][1]=0;
   K[1][0]=0;
   K[1][1]=P[1][1]/(P[1][1]+R);

   p_v.position=p_v.position+K[0][0]*(Position-p_v.position);
   p_v.velocity=p_v.velocity+K[1][1]*(Velocity-p_v.velocity);

   P[0][0]=P[0][0]*(1-K[0][0]);
   P[0][1]=P[0][1]*(1-K[0][0]);
   P[1][0]=P[1][0]*(1-K[1][1]);
   P[1][1]=P[1][1]*(1-K[1][1]);

//--------------------------------



  return p_v;

}
Vector3d SINS::Quaternion_Rotation(Vector3d item,float pitch,float yaw,float roll) //四元数旋转
{

    Quaterniond  qx( cos(PI*roll/360) ,sin(PI*roll/360),0,0);
    Quaterniond  qy( cos(PI*pitch/360) , 0,sin(PI*pitch/360),0);
    Quaterniond  qz( cos(PI*yaw/360) , 0,0,sin(PI*yaw/360));

    Vector3d AS=qx*qy*qz*item;

    return AS;

};

 QList< float> SINS::countLocation(float x,float y, float angle,float distance)
{


   location[0]=x+cos(angle*PI/180)*distance;
   location[1]=y+sin(angle*PI/180)*distance;
   return location;


};
