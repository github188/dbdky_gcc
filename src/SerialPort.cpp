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

#include <dlfcn.h>
#include <dirent.h>

#include <utility>

#include "CodecFactory.h"

using namespace std;

namespace dbdky
{
namespace detail
{
extern string getFullPath();
}
}

#if 0
namespace dbdky
{
namespace detail
{
string getFullPath()
    {   
        char current_abs_path[1024];
        int count;

        count = ::readlink("/proc/self/exe", current_abs_path, 1024);
        if (count < 0 || count >= 1024)
        {
            LOG_ERROR << "Get ABS PATH ERROR";
            return "";
        }

        current_abs_path[count] = '\0';

        string filepath(current_abs_path);
        int st = filepath.rfind("dbdky_gcc");
        if (st == string::npos)
        {
            LOG_ERROR << "GET ABS PATH ERROR:1";
            return "";
        }

        string filepath1 = filepath.substr(0, st);
        return filepath1;
    }

    list<string> getCodecLibraries()
    {
        list<string> ret;
        string currentpath = getFullPath();

        DIR *dp;
        struct dirent *entry;
        struct stat statbuf;
        if ((dp = ::opendir(currentpath.c_str())) == NULL)
        {
            LOG_ERROR << "Cannot open directory: " << currentpath;
            return ret;
        }

        ::chdir(currentpath.c_str());

        while ((entry = ::readdir(dp)) != NULL)
        {
            ::lstat(entry->d_name,&statbuf);
            if (S_ISDIR(statbuf.st_mode))
            {
                continue;
            }
            else
            {
                string filename;
                filename += entry->d_name;

                if (string::npos != filename.find(".so"))
                {
                    ret.push_back(filename);
                }
                else
                {
                    continue;
                }
            }
        }
      
        ::chdir("..");
        ::closedir(dp);

        return ret;
    }
}
}
#endif


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
      codec_(NULL),
      storeDataPtr(NULL)
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
      codec_(NULL),
      storeDataPtr(NULL)
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
    LOG_INFO << "start listen fd=" << fd_;
    unsigned char read_buf[256];
	char *dataSetValuePtr[512];
	int  dataSetTypeINT[512]={0};
	char *dataSetParamNamePtr[512];
	char *measurePointArrayPtr[64];

    int n;
    bzero(read_buf, sizeof(read_buf));
    CodecBase* pcodec = NULL;
    if (NULL == storeDataPtr)
    {
        string abspath = dbdky::detail::getFullPath();
        if (abspath.empty())
        {
            LOG_ERROR << "Can't find abspath";
            return;
        }
        string libname(abspath);
        libname += "libServerDataInput.so";

        void * hGetInstanceHandle_DataStore = ::dlopen(libname.c_str(), RTLD_NOW);
        if (NULL == hGetInstanceHandle_DataStore)
        {
            LOG_ERROR << "Can't find Data Input Instance from: " << libname;
            return;
        }

        storeDataPtr = (StoreDataFuncPtr)::dlsym(hGetInstanceHandle_DataStore, 
            "StoreData");
        if (NULL == storeDataPtr)
        {
            LOG_ERROR << "Can't get DataStoreInput interface from: " << libname;
            return;
        }
    }

    while(1)
    {
        int tmp;
        tmp = ::read(fd_, read_buf + n, sizeof(read_buf));
        if (tmp > 0)
        {
            n += tmp;
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

            dbdky::gcc::DB_INSERT_DATATYPE out;
            int retcode = pcodec->parser(read_buf, n, (void *)&out);
            if (dbdky::gcc::PARSE_SUCCESS == retcode)
            {
                LOG_INFO << "Parser OK";
                n = 0;
                //
				dbdky::gcc::DB_INSERT_DATATYPE * p = (dbdky::gcc::DB_INSERT_DATATYPE *) (&out); 
                LOG_INFO << "| ReportID = " << p->repID << ";";

				int nMesurePointNum = p->lnIDArraySize;


				for(int i = 0;i < 64;i++)
				{
					measurePointArrayPtr[i] = NULL;
				}
				for(int j = 0; j < 512;j++)
				{
					dataSetValuePtr[j]     = NULL;
					dataSetParamNamePtr[j] = NULL;
				}

				for(int i = 0;i < nMesurePointNum;i++)
				{

				    measurePointArrayPtr[i] = p->lnInstArray[i];
				    for( int i = 0;i < p->paramSize;i++ )
                    {
			            dataSetValuePtr[i] = p->dataArray[i];
                        dataSetTypeINT[i] = p->typeArray[i];
                        dataSetParamNamePtr[i] = p->fieldArray[i];
                    }
					
					storeDataPtr(p->repID, measurePointArrayPtr, nMesurePointNum, dataSetValuePtr, dataSetTypeINT, dataSetParamNamePtr, p->paramSize);
                }

                ::bzero(read_buf, sizeof(read_buf));
            }
            else if (dbdky::gcc::PARSE_ERROR_LENGTH_SHORT == retcode)
            {
                LOG_ERROR << "Length Short";
            }
            else
            {
                n = 0; 
                bzero(read_buf, sizeof(read_buf));
            }


        }
   
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
    LOG_INFO << "onQueryDataTimer: interval[" << interval << "]";
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
        
       // codec_->setConfig((*itr)->getName(), paraPair);
        MeasurePoint* mp = (*itr)->getFirstMeasurePoint();
        if (NULL == mp)
        {
            LOG_ERROR << "No measure point under this monitor unit.";
            continue;
        }

        codec_->setConfig(mp->getId(), paraPair);
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

        //if (!processPending_.waitForSeconds(5))
        //{
        //    LOG_ERROR << "op pending timeout";
        //    return;
        //}
        //else
        //{
        //    LOG_INFO << "get processPending_";
        //}

        LOG_INFO << "will Write: " << len << " Bytes to COM";
        for (int tt = 0; tt < len; tt++)
        {
            printf("0x%02X ", codedata[tt]);
        }
        int n = ::write(fd_, codedata, len);
   
        //processPending_.notify();
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
    ::tcflush(fd_, TCIOFLUSH);
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
    opt.c_iflag &= ~INPCK;
    opt.c_cflag |= (CLOCAL | CREAD);

    opt.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    opt.c_oflag &= ~OPOST;
    opt.c_oflag &= ~(ONLCR | OCRNL);

    opt.c_iflag &= ~(ICRNL | INLCR);
    opt.c_iflag &= ~(IXON | IXOFF | IXANY);
    
    ::tcflush(fd_, TCIFLUSH);
    opt.c_cc[VTIME] = 0;
    opt.c_cc[VMIN] = 0;

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
    if (name.empty() || protocolname.empty() || mac.empty())
    {
        return;
    }

    string keyname = protocolname + mac;
    map<string, boost::shared_ptr<MonitorUnit> >::const_iterator itr;
    itr = monitorUnitList_.find(keyname);
    if (monitorUnitList_.end() != itr)
    {
        return;
    }


    boost::shared_ptr<MonitorUnit> monitorunit(
         new MonitorUnit(name, interval, protocolname, mac, manufacturer, cycleid, ytime));

    monitorUnitList_.insert(make_pair<string, boost::shared_ptr<MonitorUnit> >(keyname, monitorunit));
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
        LOG_INFO << "***Monitor Counts: " << monitorUnitList_.size();
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
            LOG_INFO << "start timer :" << (*itr1)/1000;
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