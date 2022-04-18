#include "cloudpointthread.h"
QByteArray pointData;
std::mutex cpmutex;
cloudPointThread::cloudPointThread()
{

}
void cloudPointThread::run()
{


    cloudData  CD;
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
     CD.x[i]=Hex3Dec(x.toHex());
     CD.y[i]=Hex3Dec(y.toHex());
     CD.z[i]=Hex3Dec(z.toHex());
     CD.reflect[i]=Hex3Dec(R.toHex());
   }
     cpmutex.unlock();
     emit sendCloudData2GL(CD);
}
void cloudPointThread::reveiveCPFromSOCKET(QByteArray da)
{
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
