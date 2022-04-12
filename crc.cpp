#include "crc.h"
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
crc::crc()
{

}
uint16_t crc::ModbusCRC16(QByteArray senddata)
{
    int len=senddata.size();
        uint16_t wcrc=0XFFFF;//预置16位crc寄存器，初值全部为1
        //uint16_t wcrc=0X4c49;
        uint8_t temp;//定义中间变量
        int i=0,j=0;//定义计数
        for(i=0;i<len;i++)//循环计算每个数据
        {
           temp=senddata.at(i);
           wcrc^=temp;
           for(j=0;j<8;j++){
              //判断右移出的是不是1，如果是1则与多项式进行异或。
              if(wcrc&0X0001){
                  wcrc>>=1;//先将数据右移一位
                  wcrc^=0XA001;//与上面的多项式进行异或
              }
              else//如果不是1，则直接移出
                  wcrc>>=1;//直接移出
           }
        }
        temp=wcrc;//crc的值
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
         qDebug()<<"is checking...";
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
    qDebug()<<BBA.toHex();
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
    qDebug()<<QByteArray::number(FastCRC32(MESG_all,MESG_NUMBER_nCRC),16);

    if((check_crc16.toHex().toInt(&ok, 16)==FastCRC16(MESG_head,7))&&(check_crc32.toHex().toLongLong(&ok, 16)==FastCRC32(MESG_all,MESG_NUMBER_nCRC)))
    {
        checkPassed=true;
    }
    else
    {
        checkPassed=false;
    }

    checkover=true;


}

