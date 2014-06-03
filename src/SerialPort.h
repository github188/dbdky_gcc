#ifndef __SERIALPORT_H_
#define __SERIALPORT_H_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include "DataQueue.h"


namespace dbdky
{
namespace gcc
{
class SerialPort : boost::noncopyable
{
public:
	SerialPort();
	void start();
	void stop();
	void release();

	virtual ~SerialPort();
	void clearBuffer();
    
private:
	boost::shared_ptr<Parser> parser;
	boost::shared_ptr<DataQueue> inBuffer;
	int fd;
};
}
}

#endif
