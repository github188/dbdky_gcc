#ifndef __TCPCONNECTION_H_
#define __TCPCONNECTION_H_

#include <utils/Mutex.h>
#include <utils/StringPiece.h>
#include <utils/Types.h>
#include <port/Callbacks.h>
#include <port/Buffer.h>
#include <port/InetAddress.h>

#include <boost/any.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

namespace dbdky
{
namespace port
{
class Channel;
class EventLoop;
class Socket;

class TcpConnection : boost::noncopyable,
        public boost::enable_shared_from_this<TcpConnection>
{
public:
    TcpConnection(EventLoop* loop, 
	const string& name, 
	int sockfd, 
	const InetAddress& localAddr, 
	const InetAddress& peerAddr);
    ~TcpConnection();
    EventLoop* getLoop() const
    {
        return loop_;
    }

    const string& name() const
    {
        return name_;
    }

    const InetAddress& localAddress()
    {
        return localAddr_;
    }

    const InetAddress& peerAddress()
    {
        return peerAddr_;
    }

    bool connected() const
    {
        return state_ == kConnected;
    }

    void send(const void* message, size_t len);
    void send(const StringPiece& message);
    void send(Buffer* message);
    void shutdown();
    void setTcpNoDelay(bool on);
    void setContext(const boost::any& context)
    {
        context_ = context;
    }

    const boost::any& getContext() const
    {
        return context_;
    }

    boost::any* getMutableContext()
    {
        return &context_;
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

    void setHighWaterMarkCallback(const HighWaterMarkCallback& cb, size_t highWaterMark)
    {
        highWaterMarkCallback_ = cb;
        highWaterMark_ = highWaterMark;
    }

    Buffer* inputBuffer()
    {
        return &inputBuffer_;
    }

    void setCloseCallback(const CloseCallback& cb)
    {
        closeCallback_ = cb;
    }

    void connectEstablished();

    void connectDestroyed();

private:
    enum StateE { kDisconnected, kConnecting, kConnected, kDisconnecting };
    void handleRead(Timestamp receiveTime);
    void handleWrite();
    void handleClose();
    void handleError();
    void sendInLoop(const StringPiece& message);
    void sendInLoop(const void* message, size_t len);
    void shutdownInLoop();
    void setState(StateE s)
    {
        state_ = s;
    }

    EventLoop* loop_;
    string name_;
    StateE state_;
    boost::scoped_ptr<Socket> socket_;
    boost::scoped_ptr<Channel> channel_;
    InetAddress localAddr_;
    InetAddress peerAddr_;
    ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;
    WriteCompleteCallback writeCompleteCallback_;
    HighWaterMarkCallback highWaterMarkCallback_;
    CloseCallback closeCallback_;
    size_t highWaterMark_;
    Buffer inputBuffer_;
    Buffer outputBuffer_;
    boost::any context_; 
};

typedef boost::shared_ptr<TcpConnection> TcpConnectionPtr;
}
}

#endif    //__TCPCONNECTION_H_
