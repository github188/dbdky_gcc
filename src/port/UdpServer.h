#ifndef __PORT_UDPSERVER_H_
#define __PORT_UDPSERVER_H_

#include <utils/Types.h>

#include <port/EventLoop.h>
#include <port/InetAddress.h>

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/function.hpp>

#include <port/EventLoopThreadPool.h>



namespace dbdky
{
namespace port
{
class UdpServer : boost::noncopyable
{
public:
    typedef boost::function<void(EventLoop*)> ThreadInitCallback;

    UdpServer(EventLoop* loop,
	const InetAddress& listenAddr,
	const string& nameArg);

    ~UdpServer();

    const string& hostport() const
    {
        return hostport_;
    }

    const string& name() const
    {
        return name_;
    }

    EventLoop* getLoop() const
    {
        return loop_;
    }

    void setThreadNum(int numThreads);
    void setThreadInitCallback(const ThreadInitCallback& cb)
    {
        threadInitCallback_ = cb;
    }

    void start();
    void setMessageCallback(const UdpMessageCallback& cb)
    {
        messageCallback_ = cb;
    }

private:
    EventLoop* loop_;
    const string hostport_;
    const string name_;

    boost::scoped_ptr<EventLoopThreadPool> threadPool_;
    UdpMessageCallback messageCallback_;
    ThreadInitCallback threadInitCallback_;

    const InetAddress inetAddress_;

    bool started_;

    int sockfd_;
    void listen();
};
}
}

#endif    //__PORT_UDPSERVER_H_
