#include "SerialPort.h"

#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

#include <utils/Logging.h>

#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>

#include <utility>

#include "CodecFactory.h"

using namespace std;


namespace dbdky
{
namespace gcc
{
SerialPort::SerialPort(EventLoop* loop, string name, string portname, string baudrate,
        string databits, string stopbits)
    : name_(name),
      devicename_(portname),
      loop_(loop),
      threadPool_(new ThreadPool("thread" + name)),
      started_(false),
      processPending_(processMutex_),
      fd_(-1),
      codec_(NULL)
{
    config_.name_ = name;
    config_.desc_ = "";
    config_.portname_ = portname;
    config_.baudrate_ = baudrate;
    config_.databits_ = databits;
    config_.stopbits_ = stopbits;

    threadPool_->setMaxQueueSize(6);
}

SerialPort::SerialPort(EventLoop* loop, ComConfig config)
    : name_(config.name_),
      devicename_(config.portname_),
      config_(config),
      loop_(loop),
      threadPool_(new ThreadPool("thread" + name_)),
      started_(false),
      processPending_(processMutex_),
      fd_(-1),
      codec_(NULL)
{

}

SerialPort::~SerialPort()
{

}

void SerialPort::start()
{
    if (started_)
    {
        LOG_INFO << "Already started";
        return;
    }


    if (!timers_.empty())
    {
        cancelTimers();
        timers_.clear();
    }

    if (-1 == init())
    {
        LOG_ERROR << "Fail to init serial:" << devicename_;
        return;
    }

    LOG_INFO << "Init Serial Port: " << devicename_ << "->OK";

    started_ = true;
    threadPool_->start(2);

    threadPool_->run(boost::bind(&SerialPort::listen, this));

    startTimers();
    
#if 0
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

void SerialPort::listen()
{
    char read_buf[4096];
    int n;
    bzero(read_buf, sizeof(read_buf));
    CodecBase* pcodec = NULL;

    while(1)
    {
        n = ::read(fd_, read_buf, sizeof(read_buf));
        if (n > 0)
        {
            LOG_INFO << "Receive " << n << " bytes data";
            processPending_.notify();

            {
                MutexLockGuard lock(mutex_);
                pcodec = codec_;
            }

            if (NULL == pcodec)
            {
                LOG_ERROR << "No codec specified";
                n = 0;
                ::bzero(read_buf, sizeof(read_buf));
                continue;
            }

            //TODO:
            //1. Call parser:
            //2. Insert the data into database.
        }

        n = 0;
        bzero(read_buf, sizeof(read_buf));
    }
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

void SerialPort::onQueryDataTimer(int interval)
{
    list<boost::shared_ptr<MonitorUnit> > monitorUnits = getMonitorsByInterval(interval);
    list<boost::shared_ptr<MonitorUnit> >::const_iterator itr;

    for (itr = monitorUnits.begin(); itr != monitorUnits.end(); itr++)
    {
        {
            MutexLockGuard lock(mutex_);
            codec_ = CodecFactory::getInstance()->getCodec((*itr)->getProtocolName());
            if (NULL == codec_)
            {
                LOG_ERROR << "Cannot fetch codec";
                continue;
            }  
        }

        map<string, float> paraPair = (*itr)->getParamConfigList();
        codec_->setConfig(paraPair);

        unsigned char codedata[14];
        int len = 14;
        int mac;
        try
        {
            mac = boost::lexical_cast<int>((*itr)->getMac());
        }
        catch (boost::bad_lexical_cast& e)
        {
            LOG_ERROR << "Get Mac id failed";
            continue;
        }

        if (!codec_->makeQueryCmd(mac, codedata, len))
        {
            LOG_ERROR << "makeQueryCmd failed";
            continue;
        }

        if (!processPending_.waitForSeconds(5))
        {
            LOG_ERROR << "op pending timeout";
        }

        ::write(fd_, codedata, len);
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


void SerialPort::insertMonitorUnit(string name, string interval, string protocolname, 
    string mac, string manufacturer, string cycleid, string ytime)
{
    if (name.empty())
    {
        return;
    }

    map<string, boost::shared_ptr<MonitorUnit> >::const_iterator itr;
    itr = monitorUnitList_.find(name);
    if (monitorUnitList_.end() != itr)
    {
        return;
    }


    boost::shared_ptr<MonitorUnit> monitorunit(
         new MonitorUnit(name, interval, protocolname, mac, manufacturer, cycleid, ytime));

    monitorUnitList_.insert(make_pair<string, boost::shared_ptr<MonitorUnit> >(name, monitorunit));
}

MonitorUnit* SerialPort::getMonitorUnitByName(string name)
{
    if (name.empty())
    {
        return NULL;
    }

    map<string, boost::shared_ptr<MonitorUnit> >::const_iterator itr;
    itr = monitorUnitList_.find(name);
    if (monitorUnitList_.end() != itr)
    {
        return itr->second.get();
    }
    
    return NULL;
}

void SerialPort::dumpInfo() const
{
    LOG_INFO << "*****************************************";
    LOG_INFO << "SerialPort: " << name_;
    LOG_INFO << "DESC: " << config_.desc_;
    LOG_INFO << "PortName: " << config_.portname_;
    LOG_INFO << "BaudRate: " << config_.baudrate_;
    LOG_INFO << "DataBits: " << config_.databits_;
    LOG_INFO << "StopBits: " << config_.stopbits_;
    LOG_INFO << "Parity: " << config_.parity_;

    dumpMonitorUnitInfo();
}

void SerialPort::dumpMonitorUnitInfo() const
{
    map<string, boost::shared_ptr<MonitorUnit> >::const_iterator itr;

    for (itr = monitorUnitList_.begin(); itr != monitorUnitList_.end(); itr++)
    {
        itr->second->dumpInfo();
    }
}


    void SerialPort::startTimers()
    {
        cancelTimers();
        list<int> intervals;

        map<string, boost::shared_ptr<MonitorUnit> >::const_iterator itr;
        for (itr = monitorUnitList_.begin(); itr != monitorUnitList_.end(); itr++)
        {
            if (!itr->second)
            {
                continue;
            }

            string sInterval = itr->second->getInterval();
            int interval;
            try
            {
                interval = boost::lexical_cast<int>(sInterval);
            }
            catch (boost::bad_lexical_cast& e)
            {
                continue;
            }

            intervals.push_back(interval);
        }

        intervals.sort();
        intervals.unique();

        list<int>::const_iterator itr1;
        for (itr1 = intervals.begin(); itr1 != intervals.end(); itr1++)
        {
            double timeinterval = (*itr1)/1000;
            TimerId id = loop_->runEvery(timeinterval,
                boost::bind(&SerialPort::onQueryDataTimer, this, *itr1));
            timers_.insert(make_pair<int, TimerId>(*itr1, id));
        }
       
    }

    void SerialPort::cancelTimers()
    {
        map<int, TimerId>::const_iterator itr;
        for (itr = timers_.begin(); itr != timers_.end(); itr++)
        {
            loop_->cancel(itr->second);
        }

        timers_.clear();
    }

    list<boost::shared_ptr<MonitorUnit> > SerialPort::getMonitorsByInterval(int interval)
    {
        list<boost::shared_ptr<MonitorUnit> > ret;

        map<string, boost::shared_ptr<MonitorUnit> >::const_iterator itr;
        for (itr = monitorUnitList_.begin(); itr != monitorUnitList_.end(); itr++)
        {
            int monitorInterval;
            try
            {
                monitorInterval = boost::lexical_cast<int>(itr->second->getInterval());
            }
            catch (boost::bad_lexical_cast& e)
            {
                continue;
            }

            if (interval == monitorInterval)
            {
                ret.push_back(itr->second);
            }

        }

        return ret;
    }
}
}