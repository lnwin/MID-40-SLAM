#include "cloudpointthread.h"
QByteArray pointData;
std::mutex cpmutex;
int packageLength=2000;
int collectTime;
QList<float>cpx,cpy,cpz,cpreflect;
cloudPointThread::cloudPointThread()
{
   collectTime=0;
}
void cloudPointThread::run()
{

    cpmutex.lock();
    for(int i=0;i<100;i++)
   {
     QByteArray x;
     QByteArray y;
     QByteArray z;
     QByteArray R;
     x=pointData.mid(18+i,4);
     y=pointData.mid(22+i,4);
     z=pointData.mid(26+i,4);
     R=pointData.mid(30+i,1);
     std::reverse(x.begin(), x.end());
     std::reverse(y.begin(), y.end());
     std::reverse(z.begin(), z.end());
     cpx.append(Hex3Dec(x.toHex()));
     cpy.append(Hex3Dec(y.toHex()));
     cpz.append(Hex3Dec(z.toHex()));
     cpreflect.append(Hex3Dec(R.toHex()));
//     CD.y[i]=Hex3Dec(y.toHex());
//     CD.z[i]=Hex3Dec(z.toHex());
//     CD.reflect[i]=Hex3Dec(R.toHex());
     collectTime+=100;
   }
     cpmutex.unlock();
     if(collectTime==packageLength)
     {
         emit sendCloudData2GL(cpx,cpy,cpz,cpreflect);
         collectTime=0;
     }


}
void cloudPointThread::reveiveCPFromSOCKET(QByteArray da)
{
    if(collectTime==0)
    {
        cpx.clear();
        cpy.clear();
        cpz.clear();
        cpreflect.clear();
    }
    cpmutex.lock();
    pointData =da;
    cpmutex.unlock();
    this->start();
}

float cloudPointThread::Hex3Dec(QString hex)
{
      bool ok;
      float finaldata;
      long long a =hex.toLongLong(&ok,16);
      QString bin =QString::number(a,2);
      int datalength =bin.length();
      if(datalength%4==0)
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
             finaldata =(double)data.toInt(&ok,16);
             return finaldata;

         }


}
