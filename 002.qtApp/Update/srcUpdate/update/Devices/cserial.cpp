#include <fcntl.h>
#include <termios.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "cserial.h"

CSerial::CSerial()
    :fd(-1)
{
}

CSerial::~CSerial()
{
    this->close_port();
}

void CSerial::open_port(const char *file, int mode, int speed, unsigned char data_bits, unsigned char parity, unsigned char stop_bits, unsigned char flow_ctrl)
{
    if(fd != -1)
    {
        return;
    }
    if((fd = open(file, mode)) == -1)
    {
        return;
    }
    if(set_param(fd, speed, data_bits, parity, stop_bits, flow_ctrl) != 0)
    {
        this->close_port();
    }
}

/**
 * @brief Serial::set_parity
 * @param fd 文件描述符
 * @param speed 波特率：B1000000, B921600, B576000, B500000, B460800, B230400, B115200, B57600, B38400, B19200, B9600, B4800, B2400, B1200, B600, B300
 * @param data_bits 数据位 5/6/7/8
 * @param stop_bits 停止位位数：1=1个停止位、2=2个停止位
 * @param parity 校验方式：n/N=无校验、o/O=奇校验、e/E=偶校验、m/M=标号校验、s/S=空号校验
 * @param flow_ctrl 流控: o/O=无流控、x/X=software流控、h/H=hardware流控
 * @return 0-成功 -1-失败
 */
int CSerial::set_param(int fd, int speed, unsigned char data_bits, unsigned char parity, unsigned char stop_bits, unsigned char flow_ctrl)
{
    if(isatty(fd) == 0)
    {
        //fprintf(stderr, "isatty.\n");
        return -1;
    }

    struct termios opt;

    tcflush(fd, TCIOFLUSH);                     //清除所有正在发生的I/O数据

    if(tcgetattr(fd, &opt) != 0)
    {
        fprintf(stderr, "tcgetattr.\n");
        return -1;
    }

    opt.c_cflag |= (CLOCAL | CREAD);                                    //用于本地连接和接收使能
    opt.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);                     //原始输入方式，不经处理直接发送
    opt.c_iflag &= ~(PARMRK | ISTRIP | ICRNL | INLCR | IGNCR);          //屏蔽串口能把回车和换行当成同一个字符，如发送字符0X0d的时候，往往接收端得到的字符是0X0a
    opt.c_oflag &= ~(ONLCR | OCRNL);
    opt.c_oflag &= ~OPOST;

    opt.c_cc[VTIME] = 0;                        //设置超时
    opt.c_cc[VMIN] = 0;                         //define the minimum bytes data to be readed
    opt.c_cc[VINTR] = _POSIX_VDISABLE;
    opt.c_cc[VQUIT] = _POSIX_VDISABLE;
    opt.c_cc[VSTART] = _POSIX_VDISABLE;
    opt.c_cc[VSTOP] = _POSIX_VDISABLE;
    opt.c_cc[VSUSP] = _POSIX_VDISABLE;

    cfsetispeed(&opt, speed);                   //设置IO波特率
    cfsetospeed(&opt, speed);

    switch(data_bits)                           //设置数据位数
    {
    case 5:
        opt.c_cflag &= ~CSIZE;
        opt.c_cflag |= CS5;
        break;
    case 6:
        opt.c_cflag &= ~CSIZE;
        opt.c_cflag |= CS6;
        break;
    case 7:
        opt.c_cflag &= ~CSIZE;
        opt.c_cflag |= CS7;
        break;
    case 8:
        opt.c_cflag &= ~CSIZE;
        opt.c_cflag |= CS8;
        break;
    default:
        fprintf(stderr, "data_bits.\n");
        return -1;
    }

    switch(parity)                              //设置校验位
    {
    case 'n':
    case 'N':
        opt.c_cflag &= ~PARENB;                 //清除校验位
        opt.c_iflag &= ~INPCK;                  //enable parity checking
        break;
    case 'o':
    case 'O':
        opt.c_cflag |= PARENB;                  //enable parity
        opt.c_cflag |= PARODD;                  //奇校验
        opt.c_iflag |= INPCK;                   //disable parity checking
        break;
    case 'e':
    case 'E':
        opt.c_cflag |= PARENB;                  //enable parity
        opt.c_cflag &= ~PARODD;                 //偶校验
        opt.c_iflag |= INPCK;                   //disable pairty checking
        break;
    case 'm':
    case 'M':
        opt.c_cflag &= ~PARENB;                 //mark校验位
        opt.c_iflag |= INPCK;                   //disable pairty checking
        break;
    case 's':
    case 'S':
        opt.c_cflag &= ~PARENB;                 //space校验位
        opt.c_iflag |= INPCK;                   //disable pairty checking
        break;
    default:
        fprintf(stderr, "parity.\n");
        return -1;
    }

    switch(stop_bits)                           //设置停止位
    {
    case 1:
        opt.c_cflag &= ~CSTOPB;
        break;
    case 2:
        opt.c_cflag |= CSTOPB;
        break;
    default:
        fprintf(stderr, "stop_bits.\n");
        return -1;
    }

    switch(flow_ctrl) {                         //设置流控
    /*no flow control*/
    case 'o':
    case 'O':
        opt.c_cflag &= (~CRTSCTS);
        opt.c_iflag &= (~(IXON|IXOFF|IXANY));
        break;
        /*software (XON/XOFF) flow control*/
    case 'x':
    case 'X':
        opt.c_cflag &= (~CRTSCTS);
        opt.c_iflag |= (IXON|IXOFF|IXANY);
        break;
    case 'h':
    case 'H':
        opt.c_cflag |= CRTSCTS;
        opt.c_iflag &= (~(IXON|IXOFF|IXANY));
        break;
    default:
        fprintf(stderr, "flow_control.\n");
        return -1;
    }

    tcflush(fd, TCIOFLUSH);                     //清除所有正在发生的I/O数据

    if(tcsetattr(fd, TCSANOW, &opt) != 0)
    {
        fprintf(stderr, "tcsetattr.\n");
        return -1;
    }

//    struct serial_struct serial_s;
//    if(ioctl(fd, TIOCGSERIAL, &serial_s) != 0)
//    {
//        fprintf(stderr, "TIOCGSERIAL.\n");
//        return -1;
//    }
//    serial_s.flags = ASYNC_SPD_CUST;
//    serial_s.custom_divisor = serial_s.baud_base/speed;
//    if(ioctl(fd, TIOCSSERIAL, &serial_s) != 0)
//    {
//        fprintf(stderr, "TIOCSSERIAL.\n");
//        return -1;
//    }

    return 0;
}

int CSerial::read_port(unsigned char *buf, int len)
{
    if(this->fd < 0)
    {
        return -1;
    }
    return read(fd, buf, len);
}

int CSerial::write_port(unsigned char *buf, int len)
{
    if(this->fd < 0)
    {
        return -1;
    }
    return write(fd, buf, len);
}

void CSerial::close_port()
{
    if(this->fd != -1)
    {
        close(this->fd);
        this->fd = -1;
    }
}

int CSerial::get_fd()
{
    return this->fd;
}

