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

crc::crc()
{

}
uint16_t crc::ModbusCRC16(QByteArray senddata)
{
    int len=senddata.size();
        uint16_t wcrc=0XFFFF;//Ԥ��16λcrc�Ĵ�������ֵȫ��Ϊ1
        //uint16_t wcrc=0X4c49;
        uint8_t temp;//�����м����
        int i=0,j=0;//�������
        for(i=0;i<len;i++)//ѭ������ÿ������
        {
           temp=senddata.at(i);
           wcrc^=temp;
           for(j=0;j<8;j++){
              //�ж����Ƴ����ǲ���1�������1�������ʽ�������
              if(wcrc&0X0001){
                  wcrc>>=1;//�Ƚ���������һλ
                  wcrc^=0XA001;//������Ķ���ʽ�������
              }
              else//�������1����ֱ���Ƴ�
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
