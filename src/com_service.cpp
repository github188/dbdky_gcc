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
	    devmgr->start();
	}

	void com_service::stop()
	{
		
	}
}
}