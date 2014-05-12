#include <port/UdpServer.h>

#include <utils/Logging.h>

#include <boost/bind.hpp>

#include <port/SocketsOps.h>

#include <sys/socket.h>
#include <unistd.h>


using namespace dbdky;
using namespace dbdky::port;

namespace dbdky
{
namespace port
{
namespace detail
{
typedef struct sockaddr SA;

const SA* sockaddr_cast(const struct sockaddr_in* addr)
{
    return static_cast<const SA*>(implicit_cast<const void*>(addr));
}

SA* sockaddr_cast(struct sockaddr_in* addr)
{
    return static_cast<SA*>(implicit_cast<void*>(addr));
}
}
}
}

namespace dbdky
{
namespace port
{
void defaultUdpMessageCallback(dbdky::port::Buffer* buffer, dbdky::Timestamp receiveTime)
{
    LOG_INFO << "";
}
}
}

UdpServer::UdpServer(EventLoop* loop,
	const InetAddress& listenAddr,
	const string& nameArg)
	: loop_(CHECK_NOTNULL(loop)),
          inetAddress_(listenAddr),
	  hostport_(listenAddr.toIpPort()),
	  name_(nameArg),
          threadPool_(new EventLoopThreadPool(loop)),
	  messageCallback_(defaultUdpMessageCallback),
	  started_(false),
          sockfd_(0)
{
}

UdpServer::~UdpServer()
{
    //TODO:
}

void UdpServer::setThreadNum(int numThreads)
{
    assert(0 <= numThreads);
    threadPool_->setThreadNum(numThreads);
}

void UdpServer::start()
{
    if (!started_)
    {
        started_ = true;
        threadPool_->start(threadInitCallback_);
    }

    sockfd_ = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd_ < 0)
    {
        LOG_SYSFATAL << "create socket";
    }

    int ret = ::bind(sockfd_, detail::sockaddr_cast(&(inetAddress_.getSockAddrInet())), static_cast<socklen_t>(sizeof inetAddress_.getSockAddrInet()));
    if (ret < 0)
    {
        LOG_SYSFATAL << "bind socket";
    }

    loop_->runInLoop(
        boost::bind(&UdpServer::listen, this));
    
}

void UdpServer::listen()
{
    char buff[4096];
    
    loop_->assertInLoopThread();
    struct sockaddr_in clientAddr;
    socklen_t len = sizeof(clientAddr);

    while (1)
    {
        LOG_INFO << "sockfd_[" << sockfd_ << "]";
        ssize_t n = ::recvfrom(sockfd_, buff, 4095, 0, (struct sockaddr*)&clientAddr, &len);
        if (n > 0)
        {
            buff[n] = 0;
            LOG_INFO << "Receive Data from: " << ::inet_ntoa(clientAddr.sin_addr) << "Port: " << ntohs(clientAddr.sin_port) << "Data: " << buff;
            if (messageCallback_)
            {
                messageCallback_(NULL, Timestamp::now());
            }
        }
        else
        {
            LOG_ERROR << "Receive Error.[n]:" << n;
            
        }
    }
}



