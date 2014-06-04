#ifndef __SERIALPORT_H_
#define __SERIALPORT_H_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

#include <string>


#include <port/EventLoop.h>
#include <utils/ThreadPool.h>
#include <port/Callbacks.h>

#include <pdu.h>



using std::string;
using namespace dbdky;
using namespace dbdky::port;

namespace dbdky
{
namespace gcc
{
class SerialPort : boost::noncopyable
{
public:
	SerialPort(EventLoop* loop, string devicename, uint16_t queryInterval = 5, 
		pdu::ProtocolType type=pdu::kUnknown);
	void start();
	void stop();
	void release();

    void listen();

	virtual ~SerialPort();
	void clearBuffer();
    
private:
    int init();

	int fd_;
    string devicename_;
    EventLoop* loop_;
    boost::scoped_ptr<ThreadPool> threadPool_;

    TimerId queryDataTimer_;
    uint16_t queryInterval_;
    void onQueryDataTimer();
    bool started_;
    pdu::ProtocolType pType_;
};
}
}

#endif
