#ifndef CRC_H
#define CRC_H

#include <QObject>

class crc:public QObject
{
    Q_OBJECT
public:
    crc();
    uint16_t ModbusCRC16(QByteArray senddata);
    uint16_t FastCRC16(uint16_t crc,const uint8_t *data, uint16_t len);

    // crc16 table



};

#endif // CRC_H
