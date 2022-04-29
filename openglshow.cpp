#include "openglshow.h"
const float PI =3.1415926;
//==========================camera
float GLwidth,GLheight;
glm::vec3 cameraPos = glm::vec3(100.0f, 0.0f, 50.0f);
float FOV=45;
float yaw=0,pitch=0,roll=0;//????¦Ë??
float Ddx=0,Ddy=0;
float R=1000;
float Center_x=0,Center_y=0,Center_z=300;
float Target_x=0, Target_y=0,Target_z=0;
int mousedond_x,mousedond_y;
float xoffset, yoffset,zoffset;
float viewDistance;
bool mousebutton_left;
bool mousebutton_right;
bool mousebutton_mid;
bool mousebutton_double=false;
bool firstmouse=true;
bool isWheeling =true;
QMouseEvent *connectEvent;
std::mutex MTX;
//=========================camera
static M3DVector3f cloudP[99999999];

QList<float>px,py,pz,preflect;

int  cornernumber=0;
openglShow::openglShow()
{

  Csins=new SINS();
}
openglShow::~openglShow()
{

}
openglShow::openglShow(QWidget *parent) : QOpenGLWidget(parent)
{
  // connect(this,SIGNAL(wheel2Update(QMouseEvent *event)),this,SLOT(mouseReleaseEvent(QMouseEvent *event)));
}
int CCC=0;
void openglShow::receivePointCloud(cloudData Cd)
{


    MTX.lock();
    for (int i=0;i<2000;i++)
    {
        px.append(Cd.x[i]);
        py.append(Cd.y[i]);
        pz.append(Cd.z[i]);
        preflect.append(Cd.reflect[i]);
//        preflect.append(Cd.reflect[i]);
        cloudP[i+cornernumber][0]=Cd.x[i];
        cloudP[i+cornernumber][1]=Cd.y[i];
        cloudP[i+cornernumber][2]=Cd.z[i];
//        px[i]= (Cd.x[i]);
//        px[i]=(Cd.y[i]);
//        px[i]=(Cd.z[i]);

    }
    cornernumber +=2000;
    MTX.unlock();
    this->update();

};
void  openglShow::initializeGL()
{

    initializeOpenGLFunctions();
   // loadGLTextures();
    glEnable(GL_TEXTURE_2D);//????????
    glShadeModel(GL_SMOOTH);//????????????
    glEnable(GL_DEPTH_TEST);//???????????
    glDepthFunc(GL_LEQUAL);//???????????????
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);//???????§µ??
  //  glBlendFunc(GL_SRC_ALPHA, GL_ONE);//????????????alpha????????????????????1.0
    glEnable(GL_BLEND);
   //===========================????????
//============================
   this->setWindowTitle("Cloud Points");
   glViewport(0, 0, GLwidth, GLheight);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(FOV, (float)GLwidth/(float)GLheight, 1.0, 3000000.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

};
void  openglShow::paintGL()
{

    M3DMatrix44f mat_proj, mat_modelview;

    int width = GLwidth, height = GLheight;
    glClear (GL_COLOR_BUFFER_BIT); //??????????????
   // glPushAttrib(GL_POLYGON_BIT);
   // glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);//???????????
   //   glClearColor(0.5f, 0.8f, 0.4f, 0.0f);
    glViewport (0, 0, (GLsizei) width, (GLsizei) height); // ???????????

    glGetIntegerv(GL_VIEWPORT, viewport);

    glMatrixMode (GL_PROJECTION);// ?????????
    glLoadIdentity (); // ??????????
    gluPerspective(FOV, (GLfloat) width/(GLfloat) height, 1.0, 30000); // ?????????§³

    glGetFloatv(GL_PROJECTION_MATRIX, mat_proj);

    glMatrixMode(GL_MODELVIEW);// ????????????
    glLoadIdentity(); // ??????????
    gluLookAt(cameraPos.x,cameraPos.y,cameraPos.z,  0,0,0,  0, 1, 0);//??????????????????¦Ë?¨¢?????????????????????????????¦Ë?¨¢????????????????????????????????????????????????‰Î??
    glRotatef(0,1,0.0,0.0);
    glRotatef(0,0.0,1,0.0);
    glPushMatrix();

    //?????????????
    glGetFloatv(GL_MODELVIEW_MATRIX, mat_modelview);
    //=====================paint cloud point
    glPointSize(1);
    glBegin(GL_POINTS);
    MTX.lock();
//    if(px.length()>70)
//    {
    for (int i = 0; i <cornernumber; i++)
    {
      // glVertex3f(px[i],py[i],pz[i]);
      // glVertex4f()
        glVertex3fv(cloudP[i]);
    }
//    }
    MTX.unlock();
    glEnd();
    //=====================
    //=====================  paint A ball
    drawball(0,0,0,20,10,10);
    glLineWidth(3);
    glBegin(GL_LINES);      //  ????
    glColor3f (1.0f, 0.0f, 0.0f);
    glVertex3f(0, 0, 0 );           //  ???????¦Å?A X
    glVertex3f(300, 0,0);           //  ???????¦Å?B
    glColor3f (1.0f, 1.0f, 0.0f);
    glVertex3f(0, 0,0);             //  ???????¦Å?A Y
    glVertex3f(0, 300,0);           //  ???????¦Å?B
    glColor3f (0.0f, 1.0f, 0.0f);
    glVertex3f(0,0,0);              //  ???????¦Å?A Z
    glVertex3f(0,0,300);
    glEnd();
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();
    glPopAttrib();
    //=====================
    this->update();


};
void  openglShow::resizeGL(int w, int h)
{
    GLwidth=w;
    GLheight=h;
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluPerspective(60.0,w/h,0.1,100000);//???¨´?????
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
};
void  openglShow::mousePressEvent(QMouseEvent *event)
{


       if(firstmouse)
       {
           connectEvent=event;
           firstmouse=false;
           cameraPos.x = Center_x;
           cameraPos.y = Center_y;
           cameraPos.z = Center_z+R;
       }

       if(event->button()==Qt::LeftButton)
       {
         mousedond_x=event->x();
         mousedond_y=event->y();
         mousebutton_left=true;
       }

      else
       {
        mousedond_x=event->x();
        mousedond_y=event->y();
        mousebutton_right=true;
       }

       this->update();

};
void  openglShow::mouseReleaseEvent(QMouseEvent *event)
{
    Ddx=yaw;
    Ddy=pitch;

    mousebutton_left=false;
    mousebutton_right=false;

    this->update();
};
void  openglShow::mouseMoveEvent(QMouseEvent *event)
{
    yaw     = Ddx+(event->x()-mousedond_x)*0.2;
    pitch   = Ddy+(event->y()-mousedond_y)*0.2;
    if(pitch > 89.0f)
      pitch =  89.0f;
    if(pitch < -89.0f)
      pitch = -89.0f;

    cameraPos.y =sin(glm::radians(pitch))*R+Target_y;
    cameraPos.x= R*cos(glm::radians(pitch))* cos(glm::radians(yaw)) +Target_x;
    cameraPos.z= R*cos(glm::radians(pitch))*sin(glm::radians(yaw))+Target_z;

//    Vector3d CAM(cameraPos.x,cameraPos.y, cameraPos.z);
////    CAM.x()= cameraPos.x;
////    CAM.y()= cameraPos.y;
////    CAM.z()= cameraPos.z;

//    Vector3d AK = Csins->Quaternion_Rotation(CAM,pitch,yaw,roll);

//    cameraPos.x=AK.x()+Target_y;
//    cameraPos.y=AK.y()+Target_x;
//    cameraPos.z=AK.z()+Target_z;

    this->update();
};
void  openglShow::wheelEvent(QWheelEvent*event)
{

    if(event->delta()>0)
     {

    // CP_Z=CDZ-=event->delta()*10;

           R-=event->angleDelta().y()*0.001*R;



     }
     if(event->angleDelta().y()<0)
     {

    //  CP_Z=CDZ-=event->delta()*10;
         R-=event->delta()*0.001*R;

     }
     cameraPos.y =sin(glm::radians(pitch))*R+Target_y;
 //  cameraPos.z= sin(glm::radians(yaw)) * cos(glm::radians(pitch))*30000;
     cameraPos.x= R*cos(glm::radians(pitch))* cos(glm::radians(yaw)) +Target_x;
     cameraPos.z= R*cos(glm::radians(pitch))*sin(glm::radians(yaw))+Target_z;
     this->update();

};
void  openglShow::mouseDoubleClickEvent(QMouseEvent*event)
{

};
void  openglShow::drawball(float cx,float cy,float cz,float radius,float M,float N)
{


      float step_z = PI / M;
      float step_xy = 2 * PI / N;
      float x[4], y[4], z[4];

      float angle_z = 0.0;
      float angle_xy = 0.0;
      int i = 0, j = 0;
      glBegin(GL_QUADS);


      for(i = 0; i < M; i++)
      {
          angle_z = i * step_z;

          for(j = 0; j < N; j++)
          {
              angle_xy = j * step_xy;

              x[0] = radius * sin(angle_z) * cos(angle_xy);
              y[0] = radius * sin(angle_z) * sin(angle_xy);
              z[0] = radius * cos(angle_z);

              x[1] = radius * sin(angle_z + step_z) * cos(angle_xy);
              y[1] = radius * sin(angle_z + step_z) * sin(angle_xy);
              z[1] = radius * cos(angle_z + step_z);

              x[2] = radius * sin(angle_z + step_z) * cos(angle_xy + step_xy);
              y[2] = radius * sin(angle_z + step_z) * sin(angle_xy + step_xy);
              z[2] = radius * cos(angle_z + step_z);

              x[3] = radius * sin(angle_z) * cos(angle_xy + step_xy);
              y[3] = radius * sin(angle_z) * sin(angle_xy + step_xy);
              z[3] = radius * cos(angle_z);
              for(int k = 0; k < 4; k++)
              {
                  glColor3f(1, 0, 0);
                  glVertex3f(cx + x[k], cy + y[k], cz + z[k]);
              }
          }
      }
      glEnd();

};

void openglShow::savecloud()
{
     QString  filepath="D:/clouds/mid40cloud.txt";
       QFile cloudfile(filepath);
       QTextStream stream(&cloudfile);
       if(!cloudfile.exists())

               {
                    QDir *folder = new QDir;
                    folder->mkdir("D:/clouds");
                    cloudfile.open(QIODevice::WriteOnly | QIODevice::Text);
                    for( int i=0;i<px.length(); i++)
                    {
                         stream<< QString::number( px[i]) +","+QString::number( py[i])+","+QString::number(pz[i])+","+QString::number(preflect[i])<<"\n";

                    }

                }
       else
       {
           cloudfile.open(QIODevice::WriteOnly | QIODevice::Text|QIODevice::Append);
           for( int i=0;i<px.length(); i++)
           {
               stream<< QString::number( px[i]) +","+QString::number( py[i])+","+QString::number(pz[i])+","+QString::number(preflect[i])<<"\n";


            }

       }
       cloudfile.close();
}
