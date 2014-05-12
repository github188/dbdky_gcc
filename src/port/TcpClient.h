#ifndef __PORT_TCPCLIENT_H_
#define __PORT_TCPCLIENT_H_

#include <boost/noncopyable.hpp>

#include <utils/Mutex.h>
#include <port/TcpConnection.h>

namespace dbdky
{
namespace port
{

class Connector;
typedef boost::shared_ptr<Connector> ConnectorPtr;

class TcpClient : boost::noncopyable
{
public:
    TcpClient(EventLoop* loop, 
            const InetAddress& serverAddr,
            const string& name);
    ~TcpClient();

    void connect();
    void disconnect();
    void stop();

    TcpConnectionPtr connection() const
    {
        MutexLockGuard lock(mutex_);
        return connection_;
    }

    EventLoop* getLoop() const
    {
        return loop_;
    }
  
    bool retry() const;
    void enableRetry()
    {
        retry_ = true;
    }

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
    void newConnection(int sockfd);
    void removeConnection(const TcpConnectionPtr& conn);
   
    EventLoop* loop_;
    ConnectorPtr connector_;
    const string name_;
    ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;
    WriteCompleteCallback writeCompleteCallback_;
    bool retry_;
    bool connect_;
    int nextConnId_;
    mutable MutexLock mutex_;
    TcpConnectionPtr connection_;
};
}
}


#endif    //__PORT_TCPCLIENT_H_
