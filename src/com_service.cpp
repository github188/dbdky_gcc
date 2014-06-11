#include "com_service.h"

#include <boost/bind.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include <utils/Logging.h>
#include <utils/Timestamp.h>

#include <vector>

#include "DeviceManager.h"

using namespace std;

namespace dbdky
{
namespace gcc
{
	com_service::com_service(EventLoop* loop,
		const string& name)
	: loop_(loop),
	  name_(name)
	{

	}

	com_service::~com_service()
	{

	}

	void com_service::start()
	{
		DeviceManager* devmgr = DeviceManager::getInstance(loop_);
		LOG_INFO << "ooo";
	    devmgr->start();
	    //DeviceManager::getInstance(loop_)->dumpSerialPortsInfo();
	    
		//DeviceManager::getInstance(loop_)->dumpSerialPortsInfo();
		// vector<string> files = DeviceManager::getInstance(loop_)->getFiles();
		// LOG_INFO << files.size();
		// vector<string>::const_iterator itr;
		// for (itr = files.begin(); itr != files.end(); itr++)
		// {
		// 	LOG_INFO << "File: " << *itr;
		// }
		// SerialPort port(loop_, "/dev/pts/14");
		// port.start();
	}

	void com_service::stop()
	{
		
	}
}
}