#ifndef CUPDATEMODULE_H
#define CUPDATEMODULE_H

#include "../Utilities/includes.h"


enum SF_UPDATE_STATUS
{
    SF_UPDATE_IDEL          = 0x00,
    SF_UPDATE_SEND_TAG,
    SF_UPDATE_RECEIVE_TAG,
    SF_UPDATE_SEND_LEN,
    SF_UPDATE_RECEIVE_LEN,
    SF_UPDATE_SEND_DATA,
    SF_UPDATE_RECEIVE_DATA,
    SF_UPDATE_SEND_END,
    SF_UPDATE_RECEIVE_END,
    SF_UPDATE_SEND_DONE,
    SF_UPDATE_RECEIVE_DONE,
    SF_UPDATE_ERROR,
};

enum UPDATE_COMMAND
{
    UPDATE_ACK       = 0x01,
    UPDATE_NAK,
    UPDATE_EOT,
    UPDATE_SOL, //start of *.bin lenght
    UPDATE_SOD, //start of data:length = packet data lenght - 2(CID + Number)
    UPDATE_CA,  //one of these in succession aborts transfer
    UPDATE_RP,  //resend the packet
    UPDATE_W_F_DONE,  //Write data into flash done
    UPDATE_W_F_ERR,  //Write data into flash error
};

class CUpdateModule
{
public:
    CUpdateModule(CModuleBase *pParent);
    ~CUpdateModule();

    int initApplication(const UINT8 &target,
                        const QString &strBinFilePath);
    void doUpdateProcess(const CAnalysisData &data);
    void sendUpdateTag(void);

protected:

    int  readFile2RAM(const QString &strBinFilePath);

private:
    CModuleBase         *m_pParent;
    SF_UPDATE_STATUS    m_Status;
    CAnalysisData       m_SendData;
    UINT32              m_BinFileLen;
    UINT32              m_BinFileOffset;
    UINT32              m_PacketCount;
    UINT8               *m_pBinFile;

};

#endif // CUPDATEMODULE_H
