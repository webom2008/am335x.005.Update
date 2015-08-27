#ifndef CDATAFRAME_H
#define CDATAFRAME_H

#include "../Utilities/canalysisdata.h"

//串口数据解析状态
enum AnalysisStatus{
    WaitDstAddr,
    WaitSrcAddr,
    WaitSN,
    WaitType,
    WaitLen,
    WaitData,
    WaitCRC
};

class CDataFrame
{
public:
    CDataFrame(unsigned char host_addr, unsigned char slave_addr);
    ~CDataFrame();

    CAnalysisData& get_data();
    unsigned char get_slave_addr();

    int unpack(CAnalysisData &txData, unsigned char *buf, int len);
    int pack(const unsigned char *buf, int len);

private:
    int unpack_AIO(CAnalysisData &txData, unsigned char *buf, int len);
    int pack_AIO(const unsigned char *buf, int len);
    unsigned char crc8_AIO(const CAnalysisData &mData);

    int unpack_data(CAnalysisData &txData, unsigned char *buf, int len);
    int pack_data(const unsigned char *buf, int len);
    unsigned char crc8_data(const CAnalysisData &mData);

private:
    const unsigned char slaveAddr;
    const unsigned char hostAddr;
    CAnalysisData rxData;
    AnalysisStatus mStatus;
    int dataLen;
};

#endif // CDATAFRAME_H
