#ifndef __SERIALPORT_H_
#define __SERIALPORT_H_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <string>

using std::string;

namespace dbdky
{
namespace gcc
{
class SerialPort : boost::noncopyable
{
public:
	SerialPort(string& devicename);
	void start();
	void stop();
	void release();

	virtual ~SerialPort();
	void clearBuffer();
    
private:
        int init();
	int fd_;
        string devicename_;
};
}
}

#endif
