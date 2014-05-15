#include "com_service.h"

#include <boost/bind.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include <utils/Logging.h>
#include <utils/Timestamp.h>


namespace dbdky
{
namespace gcc
{
	com_service::com_service(EventLoop* loop,
		const string& name)
	: loop_(loop),
	  name_(name),
	  threadPool_(new EventLoopThreadPool(loop))
	{
        threadPool_->setThreadNum(10);
        threadPool_->start();
	}

	com_service::~com_service()
	{

	}

	void com_service::start()
	{

	}

	void com_service::stop()
	{
		
	}
}
}