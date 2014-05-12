#ifndef __PORT_TCPSERVER_H_
#define __PORT_TCPSERVER_H_

#include <utils/Types.h>
#include <port/TcpConnection.h>

#include <map>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

namespace dbdky
{
namespace port
{
class Acceptor;
class EventLoop;
class EventLoopThreadPool;

class TcpServer : boost::noncopyable
{
public:
    typedef boost::function<void(EventLoop*)> ThreadInitCallback;
    enum Option
    {
        kNoReusePort,
        kReusePort,
    };

    TcpServer(EventLoop* loop, 
            const InetAddress& listenAddr,
            const string& nameArg,
            Option option = kNoReusePort);

    ~TcpServer();

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
    void setConnectionCallback(const ConnectionCallback& cb)
    {
        connectionCallback_ = cb;
    }

    void setMessageCallback(const MessageCallback& cb)
    {
        messageCallback_ = cb;
    }
  
    void setWriteCompleteCallback(const WriteCompleteCallback& cb)
    {
        writeCompleteCallback_ = cb;
    }

private:
    void newConnection(int sockfd, const InetAddress& peerAddr);
    void removeConnection(const TcpConnectionPtr& conn);
    void removeConnectionInLoop(const TcpConnectionPtr& conn);

    typedef std::map<string, TcpConnectionPtr> ConnectionMap;

    EventLoop* loop_;
    const string hostport_;
    const string name_;
    boost::scoped_ptr<Acceptor> acceptor_;
    boost::scoped_ptr<EventLoopThreadPool> threadPool_;
    ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;
    WriteCompleteCallback writeCompleteCallback_;
    ThreadInitCallback threadInitCallback_;
    bool started_;
    int nextConnId_;
    ConnectionMap connections_;
};
}
}


#endif    //__PORT_TCPSERVER_H_
