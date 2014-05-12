#ifndef __PORT_ACCEPTOR_H_
#define __PORT_ACCEPTOR_H_

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>

#include <port/Channel.h>
#include <port/Socket.h>

namespace dbdky
{
namespace port
{
class EventLoop;
class InetAddress;

class Acceptor : boost::noncopyable
{
public:
    typedef boost::function<void (int sockfd,
                        const InetAddress&)> NewConnectionCallback;

    Acceptor(EventLoop* loop, const InetAddress& listenAddr, bool reuseport);

    ~Acceptor();

    void setNewConnectionCallback(const NewConnectionCallback& cb)
    {
        newConnectionCallback_ = cb;
    }

    bool listenning() const
    {
        return listenning_;
    }

    void listen();

private:
    void handleRead();

    EventLoop* loop_;
    Socket acceptSocket_;
    Channel acceptChannel_;
    NewConnectionCallback newConnectionCallback_;
    bool listenning_;
    int idleFd_;
};
}
}

#endif    //__PORT_ACCEPTOR_H_
