#include <stdlib.h>

#include "cmodulebase.h"
#include "../Devices/cserialanalysis.h"

CModuleBase::CModuleBase(CSerialControl *pSerialControl,
                         CSerialAnalysis *pAnalysis)
    :m_pSerialControl(pSerialControl)
    ,m_pAnalysis(pAnalysis)
{

}

CModuleBase::~CModuleBase()
{

}
