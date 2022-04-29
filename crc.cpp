#include "crc.h"
#include <mutex>
#include <ui_mainwindow.h>
#define crc_n4(crc, data, table) crc ^= data; \
    crc = (table[(crc & 0xff) + 0x300]) ^		\
          (table[((crc >> 8) & 0xff) + 0x200]) ^	\
          (table[((data >> 16) & 0xff) + 0x100]) ^	\
          (table[data >> 24]);
#define crcsm_n4d(crc, data, table) crc ^= data; \
    crc = (crc >> 8) ^ (table[crc & 0xff]); \
    crc = (crc >> 8) ^ (table[crc & 0xff]); \
    crc = (crc >> 8) ^ (table[crc & 0xff]); \
    crc = (crc >> 8) ^ (table[crc & 0xff]);
#define CRC_TABLE_CRC32 crc_table_crc32
bool checkPassed=false;
bool checkover=false;

std::mutex mtx;

crc::crc()
{

}
uint16_t crc::ModbusCRC16(QByteArray senddata)
{
    int len=senddata.size();
        uint16_t wcrc=0XFFFF;//Ԥ��16λcrc�Ĵ�������ֵȫ��Ϊ1
        //uint16_t wcrc=0X4c49;
        uint8_t temp;//�����м�����
        int i=0,j=0;//��������
        for(i=0;i<len;i++)//ѭ������ÿ������
        {
           temp=senddata.at(i);
           wcrc^=temp;
           for(j=0;j<8;j++){
              //�ж����Ƴ����ǲ���1��������1��������ʽ����������
              if(wcrc&0X0001){
                  wcrc>>=1;//�Ƚ���������һλ
                  wcrc^=0XA001;//�������Ķ���ʽ��������
              }
              else//��������1����ֱ���Ƴ�
                  wcrc>>=1;//ֱ���Ƴ�
           }
        }
        temp=wcrc;//crc��ֵ
        return wcrc;

}
uint16_t crc::FastCRC16(const uint8_t *data, uint16_t len)
{

    uint16_t crc =0x4c49;
    while (((uintptr_t)data & 3) && len) {
        crc = (crc >> 8) ^ crc_table_mcrf4xx[(crc & 0xff) ^ *data++];
        len--;
    }

    while (len >= 16) {
        len -= 16;
        crc_n4(crc, ((uint32_t *)data)[0], crc_table_mcrf4xx);
        crc_n4(crc, ((uint32_t *)data)[1], crc_table_mcrf4xx);
        crc_n4(crc, ((uint32_t *)data)[2], crc_table_mcrf4xx);
        crc_n4(crc, ((uint32_t *)data)[3], crc_table_mcrf4xx);
        data += 16;
    }

    while (len--) {
        crc = (crc >> 8) ^ crc_table_mcrf4xx[(crc & 0xff) ^ *data++];
    }

    //seed = crc;
    return crc;
}
uint32_t crc::FastCRC32(const uint8_t *data, uint16_t len)
{

    uint32_t crc = 0x564f580a^0xffffffff;

    while (((uintptr_t)data & 3) && len) {
        crc = (crc >> 8) ^ CRC_TABLE_CRC32[(crc & 0xff) ^ *data++];
        len--;
    }

    while (len >= 16) {
        len -= 16;
        #if CRC_BIGTABLES
        crc_n4d(crc, ((uint32_t *)data)[0], CRC_TABLE_CRC32);
        crc_n4d(crc, ((uint32_t *)data)[1], CRC_TABLE_CRC32);
        crc_n4d(crc, ((uint32_t *)data)[2], CRC_TABLE_CRC32);
        crc_n4d(crc, ((uint32_t *)data)[3], CRC_TABLE_CRC32);
        #else
        crcsm_n4d(crc, ((uint32_t *)data)[0], CRC_TABLE_CRC32);
        crcsm_n4d(crc, ((uint32_t *)data)[1], CRC_TABLE_CRC32);
        crcsm_n4d(crc, ((uint32_t *)data)[2], CRC_TABLE_CRC32);
        crcsm_n4d(crc, ((uint32_t *)data)[3], CRC_TABLE_CRC32);
        #endif
        data += 16;
    }

    while (len--) {
        crc = (crc >> 8) ^ CRC_TABLE_CRC32[(crc & 0xff) ^ *data++];
    }

    //seed = crc;
    crc ^= 0xffffffff;

    return crc;
}
QByteArray BBA;
bool crc::checkCRC(QByteArray ba)
{

     BBA=ba;
     this->start();

     while(!checkover)
     {
        // qDebug()<<"is checking...";
     }
     if(checkPassed)
     {
         return true;
     }
     else
     {
         return false;
     }

};

void crc::run()
{
    checkover=false;
    //qDebug()<<BBA;
   // qDebug()<<BBA.toHex();
    bool ok;
    int MESG_NUMBER=BBA.mid(2,1).toHex().toInt(&ok, 16)+BBA.mid(3,1).toHex().toInt(&ok, 16)*256;
    int MESG_NUMBER_nCRC=MESG_NUMBER-4;
    uint8_t MESG_head[7];
    for(int i=0;i<7;i++)
    {
        MESG_head[i]=(uint8_t)BBA.at(i);
    }

    uint8_t MESG_all[1404];

    for(int i=0;i<MESG_NUMBER_nCRC;i++)
    {
        MESG_all[i]=(uint8_t)BBA.at(i);
    }
  //  qDebug()<<QString::number(SOC_CRC->FastCRC16(MESG_head,7), 16);
     QByteArray check_crc16;
     QByteArray check_crc32;
    check_crc16.append(BBA.at(8));
    check_crc16.append(BBA.at(7));
   // qDebug()<<check_crc16.toHex().toInt(&ok, 16);
   // qDebug()<<FastCRC16(MESG_head,7);
    check_crc32.append(BBA.at(MESG_NUMBER-1));
    check_crc32.append(BBA.at(MESG_NUMBER-2));
    check_crc32.append(BBA.at(MESG_NUMBER-3));
    check_crc32.append(BBA.at(MESG_NUMBER-4));
   // qDebug()<<check_crc32.toHex().toLongLong(&ok, 16);
   // qDebug()<<BBA.at(MESG_NUMBER-1);
   // qDebug()<<FastCRC32(MESG_all,MESG_NUMBER_nCRC);
   // qDebug()<<QByteArray::number(FastCRC32(MESG_all,MESG_NUMBER_nCRC),16);

    if((check_crc16.toHex().toInt(&ok, 16)==FastCRC16(MESG_head,7))&&(check_crc32.toHex().toLongLong(&ok, 16)==FastCRC32(MESG_all,MESG_NUMBER_nCRC)))
    {
        checkPassed=true;
    }
    else
    {
        checkPassed=false;
    }

    checkover=true;
    mtx.lock();
    analysisMsg(BBA);
    mtx.unlock();

}

void crc::analysisMsg(QByteArray data)
{

    QString HEAD =data.mid(0,1).toHex();
     if(HEAD=="aa")
    {
         QString CMD_tpye =data.mid(4,1).toHex();
         if(CMD_tpye=="02")
         {
             findDeviceMsg(data);
         }
         else if(CMD_tpye=="01")
         {
             QString CMD_SET =data.mid(9,1).toHex();

             if(CMD_SET=="00")
             {
               analysisCurrentACK(data);
             }

             else if (CMD_SET=="01")
             {
                 analysisLidarACK(data);
             }

             else
             {
                 analysisHubACK(data);
             }

        }

    }
//     else if(HEAD=="05")
//     {
//         analysisPointCloud(data);
//     }



}
void crc::findDeviceMsg(QByteArray data)
{

     if(data.size()==34)
     {

        DEVICEMSG DM;
        DM.TYPE = data.mid(27,1).toHex();
        DM.ID = data.mid(11,16).toHex();
        emit sendDeviceMSG(DM);

     }


};
void crc::analysisCurrentACK(QByteArray data)
{

     QString CMD_ID =data.mid(10,1).toHex();
    // qDebug()<<CMD_ID;
     if(CMD_ID=="01")
     {
       emit sendNeedHand(true);
     }
     else if(CMD_ID=="03")
     {
        QString ret_code =data.mid(11,1).toHex();
         if(ret_code=="00")
         {
             emit sendHandbool(true);
         }
        else
         {
             emit sendHandbool(false);
         }

     }

}
void crc::analysisLidarACK(QByteArray data)
{


};
void crc::analysisHubACK(QByteArray data)
{

};
void crc::analysisPointCloud(QByteArray data)
{
       QByteArray pointData =data;
       cloudData  CD;
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
//        if(i==99)
//        {
//             showDataOnUi(UI,CD);
//        }

      }

    //   emit sendCloudData2GL(CD);
       // emit sendCloudData2GL(CD);

}
float crc::Hex3Dec(QString hex)
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
void crc::receiveData(QByteArray ba)
{
    checkCRC(ba);
};
void crc::showDataOnUi(Ui::MainWindow ui,cloudData ba)
{
//    cloudData AK=ba;
//    qDebug()<<AK.x[0];
//    qDebug()<<AK.x[0];
//    qDebug()<<AK.x[0];
   // ui.point_x->setText(QString::number(AK.x[0]));
   // ui.point_y->setText(QString::number(AK.y[0]));
    //ui.point_z->setText(QString::number(AK.z[0]));

};
void crc::getUI(Ui::MainWindow ok)
{
    UI=ok;
}

