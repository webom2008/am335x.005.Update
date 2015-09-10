#ifndef CSERIALDEVICE_H
#define CSERIALDEVICE_H

#include "../Utilities/canalysisdata.h"
#include "../Application/fdset.h"
#include "../Devices/cdataframe.h"
#include "../Devices/cserial.h"
#include "../Utilities/templateclass.h"

#define DATA_LIST_MAX_SIZE          100
#define MAX_BUF_SIZE                1024

class CSerialControl;

class CSerialDevice
{
public:
    CSerialDevice(CSerialControl *pSerialControl,\
                  unsigned char slaveAddr,\
                  const char *file,\
                  int mode,\
                  int speed,\
                  unsigned char data_bits,\
                  unsigned char parity,\
                  unsigned char stop_bits,\
                  unsigned char flow_ctrl);
    ~CSerialDevice();

    void open_serial();
    void close_serial();

    void build_fdset(FdSet &fdSet);
    void process(FdSet &fdSet);

    bool write_txData(const CAnalysisData &txData);
    bool read_txData(CAnalysisData &txData);

protected:
    bool write_serial();
    bool read_serial();

protected:
    CSerialControl                  *m_pSerialControl;
    CDataFrame                      m_Frame;
    TemplateArray<CAnalysisData>    mTxDataArray;
    CSerial                         m_Serial;
    unsigned char   slaveAddr;
    const char      *File;
    int             Mode;
    int             Speed;
    unsigned char   Data_Bits;
    unsigned char   Stop_Bits;
    unsigned char   Parity;
    unsigned char   Flow_Ctrl;
};

#endif // CSERIALDEVICE_H
