#ifndef CANALYSISDATA_H
#define CANALYSISDATA_H

#include "Utilities/typedefs.h"

#define DATA_BUF_LEN                250

class CAnalysisData
{
public:
    CAnalysisData();

public:
    UINT8 m_DstAddr;                //目的地址
    UINT8 m_SrcAddr;                //源地址
    UINT8 m_SN;                     //包序号
    UINT8 m_Type;                   //包标识
    UINT8 m_Len;                    //数据长度
    UINT8 m_Data[DATA_BUF_LEN];     //数据段
    UINT8 m_CRC;                    //CRC校验码
    bool  m_IsNeedACK;
};

#endif // CANALYSISDATA_H
