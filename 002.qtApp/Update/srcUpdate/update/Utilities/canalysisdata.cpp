#include <string.h>
#include "canalysisdata.h"

CAnalysisData::CAnalysisData()
{
    m_DstAddr = 0;
    m_SrcAddr = 0;
    m_SN = 0;
    m_Type = 0;
    m_Len = 0;
    m_CRC = 0;
    m_IsNeedACK = false;
    memset(m_Data, 0, DATA_BUF_LEN);
}
