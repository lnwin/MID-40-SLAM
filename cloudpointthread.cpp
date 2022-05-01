#include "cloudpointthread.h"
QByteArray pointData;
std::mutex cpmutex;
int packageLength=2000;
int collectTime;
cloudData CD;
float Ayaw=0,Apitch=0,Aroll=0;
std::mutex mtcp;

cloudPointThread::cloudPointThread()
{
   collectTime=0;
   clpsins = new SINS();

}
void cloudPointThread::receiveINS(float yaw,float pitch,float roll)
{
    cpmutex.lock();
    Ayaw=yaw;
    Apitch=pitch;
    Aroll=roll;
   // qDebug()<<"Ayaw"<<yaw;
    cpmutex.unlock();
}
void cloudPointThread::run()
{


    cpmutex.lock();


    for(int i=0;i<100;i++)
   {
     QByteArray x,y,z,R;
     x=pointData.mid(18+i*13,4);
     y=pointData.mid(22+i*13,4);
     z=pointData.mid(26+i*13,4);
     R=pointData.mid(30+i*13,1);
     std::reverse(x.begin(), x.end());
     std::reverse(y.begin(), y.end());
     std::reverse(z.begin(), z.end());
//   cpx.append(Hex3Dec(x.toHex()));
//   cpy.append(Hex3Dec(y.toHex()));
//   cpz.append(Hex3Dec(z.toHex()));
     Vector3d origin;
     origin.x()=Hex3Dec(x.toHex());
     origin.y()=Hex3Dec(y.toHex());
     origin.z()=Hex3Dec(z.toHex());
     Vector3d outPut;
     outPut=clpsins->Quaternion_Rotation(origin,Apitch,Ayaw,Aroll);

//   cpreflect.append(Hex3Dec(R.toHex()));
     CD.x[collectTime]=outPut.x();
     CD.y[collectTime]=outPut.y();
     CD.z[collectTime]=outPut.z();


     bool ok;
     CD.reflect[collectTime]=R.toHex().toLongLong(&ok,16);
   //  qDebug()<<Hex3Dec(x.toHex());



     collectTime+=1;
     if(collectTime==packageLength)
     {
        // qDebug()<<"ok"<<collectTime;
         emit sendCloudData2GL(CD);
       //  qDebug()<<CD.reflect[0];
         Delay_MSec(20);
         collectTime=0;
     }

   }


     cpmutex.unlock();
    // done=true;



}
void cloudPointThread::reveiveCPFromSOCKET(QByteArray da)
{

    cpmutex.lock();
    pointData =da;
    cpmutex.unlock();
    this->start();

   // qDebug()<<"RCP";
}

float cloudPointThread::Hex3Dec(QString hex)
{
    bool ok;
    float finaldata;
    long long a =hex.toLongLong(&ok,16);
    QString bin =QString::number(a,2);
    int datalength =bin.length();
   // qDebug()<<bin.length();
  //  qDebug()<<bin;

        if(datalength==32)
        {
           for (int i=0;i<datalength;i++)
           {
               if(bin[i]=="0")
               {
                   bin[i]='1';
               }
               else
               {
                   bin[i]='0';
               }

           }

           finaldata = (double)(-(bin.toInt(&ok,2)+1));
           return finaldata;

        }



       else
     {
           QString data =hex;
           finaldata =(double)data.toLongLong(&ok,16);
           return finaldata;

     }


}
void cloudPointThread::Delay_MSec(unsigned int msec)//-----------------------------------------��ʱ����
{
    QTime _Timer = QTime::currentTime().addMSecs(msec);

    while( QTime::currentTime() < _Timer )

    QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
}
