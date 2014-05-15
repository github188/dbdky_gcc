#ifndef __COM_SERVICE_H_
#define __COM_SERVICE_H_

#include <port/EventLoop.h>

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <boost/scoped_ptr.hpp>

#include <port/Callbacks.h>
#include <port/Buffer.h>
#include <utils/Timestamp.h>

#include <port/EventLoop.h>
#include <port/EventLoopThreadPool.h>

#include <string>

using namespace dbdky;
using namespace dbdky::port;

using std::string;

namespace dbdky
{
namespace gcc
{
class com_service : boost::noncopyable
{
public:
	explicit com_service(EventLoop* loop, const string& name);
	void start();
	void stop();

	virtual ~com_service();

private:
	EventLoop* loop_;
	TimerId systemTimer_;

	const string name_;

	boost::scoped_ptr<EventLoopThreadPool> threadPool_;
};
}
}
#endif