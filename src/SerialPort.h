#ifndef __SERIALPORT_H_
#define __SERIALPORT_H_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

#include <string>
#include <map>


#include <port/EventLoop.h>
#include <utils/ThreadPool.h>
#include <port/Callbacks.h>

#include <pdu.h>

#include <MonitorUnit.h>


using namespace std;
using namespace dbdky;
using namespace dbdky::port;

namespace dbdky
{
namespace gcc
{

typedef struct ComConfig
{
    string name_;
    string desc_;
    string portname_;
    string baudrate_;
    string databits_;
    string stopbits_;
    string parity_;
    string totaltimeout_;
    string intervaltimeout_;
    string inbuffersize_;
    string outbuffersize_;

} ComConfig;

class SerialPort : boost::noncopyable
{
public:
	//SerialPort(EventLoop* loop, string devicename, uint16_t queryInterval = 5, 
//		pdu::ProtocolType type=pdu::kUnknown);
    SerialPort(EventLoop* loop, string name, string portname, string baudrate="9600",
        string databits="8", string stopbits="1");
    SerialPort(EventLoop* loop, ComConfig config);

	void start();
	void stop();
	void release();

    void listen();

	virtual ~SerialPort();
	void clearBuffer();

    void insertMonitorUnit(string name, string interval, string protocolname, string mac, 
        string manufacturer="", string cycleid="", string ytime="");

    MonitorUnit* getMonitorUnitByName(string name);

    void dumpInfo() const;

private:
    int init();

	int fd_;
    string name_;
    string devicename_;
    EventLoop* loop_;
    ComConfig config_;
    boost::scoped_ptr<ThreadPool> threadPool_;

    TimerId queryDataTimer_;
    //uint16_t queryInterval_;
    void onQueryDataTimer();
    bool started_;
    //pdu::ProtocolType pType_;

    map<string, boost::shared_ptr<MonitorUnit> > monitorUnitList_;
};
}
}

#endif
