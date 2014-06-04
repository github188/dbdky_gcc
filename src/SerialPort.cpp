#include "SerialPort.h"

#include <utils/Logging.h>

#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>

using namespace dbdky;
using namespace dbdky::gcc;

SerialPort::SerialPort(string& devicename)
    : devicename_(devicename),
      fd_(-1)
{
}

SerialPort::~SerialPort()
{

}

void SerialPort::start()
{
    if (-1 == init())
    {
        LOG_ERROR << "Fail to init serial:" << devicename_;
        return;
    }

#if 1
    char read_buf[4096];
    int n;
    //Buffer buf;
    bzero(read_buf, sizeof(read_buf));
    while (1)
    {
        n = ::read(fd_, read_buf, sizeof(read_buf));
        if (n > 0)
        {
            //buf.append(read_buf, n);
            LOG_INFO << "Receive " << n << " bytes data";
        }
        n = 0;
        bzero(read_buf, sizeof(read_buf));
    }
#endif
}

void SerialPort::stop()
{

}

void SerialPort::release()
{
    if (fd_ != -1)
    {
        ::close(fd_);
        fd_ = -1;
    }
}

int SerialPort::init()
{
    struct termios opt;

    if (fd_ >= 0)
    {
        LOG_INFO << "No need to init";
        return fd_;
    }

    fd_ = ::open(devicename_.c_str(), O_RDWR | O_NOCTTY);
    if (-1 == fd_)
    {
        LOG_ERROR << "open serial failed: " << devicename_;
        return fd_;
    }    
    
    ::tcgetattr(fd_, &opt);
    ::cfsetispeed(&opt, B9600);
    ::cfsetospeed(&opt, B9600);
    
    if (::tcsetattr(fd_, TCSANOW, &opt) != 0)
    {
        LOG_ERROR << "tcsetattr error";
        goto _fail;
    }

    opt.c_cflag &= ~CSIZE;
    opt.c_cflag |= CS8;
    opt.c_cflag &= ~CSTOPB;
    opt.c_cflag &= ~PARENB;
    opt.c_cflag &= ~INPCK;
    opt.c_cflag |= (CLOCAL | CREAD);

    opt.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    opt.c_oflag &= ~OPOST;
    opt.c_oflag &= ~(ONLCR | OCRNL);

    opt.c_iflag &= ~(ICRNL | INLCR);
    opt.c_iflag &= ~(IXON | IXOFF | IXANY);

    opt.c_cc[VTIME] = 0;
    opt.c_cc[VMIN] = 0;

    ::tcflush(fd_, TCIOFLUSH);

    LOG_INFO << "Configure complete";

    if (::tcsetattr(fd_, TCSANOW, &opt) != 0)
    {
        LOG_ERROR << "Serial error.";
        goto _fail;
    }

    return fd_;

_fail:
    ::close(fd_);
    fd_ = -1;
    return -1;
}

