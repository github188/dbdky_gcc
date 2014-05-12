#ifndef __SOCKET_H_
#define __SOCKET_H_

#include <boost/noncopyable.hpp>

namespace dbdky
{
namespace port
{
class InetAddress;

class Socket : boost::noncopyable
{
public:
    explicit Socket(int sockfd)
        : sockfd_(sockfd)
    {}

    ~Socket();

    int fd() const
    {
        return sockfd_;
    }

    void bindAddress(const InetAddress& localaddr);
    void listen();
    int accept(InetAddress* peeraddr);
    void shutdownWrite();
    void setTcpNoDelay(bool on);
    void setReuseAddr(bool on);
    void setReusePort(bool on);
    void setKeepAlive(bool on);

private:
    const int sockfd_;
};
}
}


#endif    //__SOCKET_H_
