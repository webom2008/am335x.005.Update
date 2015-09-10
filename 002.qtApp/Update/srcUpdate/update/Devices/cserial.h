#ifndef CSERIAL_H
#define CSERIAL_H

#include <fcntl.h>
#include <stdlib.h>
#include <termios.h>

class SerialIO;

class CSerial
{
public:
    CSerial();
    ~CSerial();

public:
    void open_port(const char *file,
                   int mode,
                   int speed,
                   unsigned char data_bits,
                   unsigned char parity,
                   unsigned char stop_bits,
                   unsigned char flow_ctrl);
    int read_port(unsigned char *buf, int len);
    int write_port(unsigned char *buf, int len);
    void close_port();
    int get_fd();

private:
    int set_param(int fd,
                  int speed,
                  unsigned char data_bits,
                  unsigned char parity,
                  unsigned char stop_bits,
                  unsigned char flow_ctrl);

private:
    int fd;
};

#endif // CSERIAL_H
