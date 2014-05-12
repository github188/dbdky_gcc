#ifndef __INETADDRESS_H_
#define __INETADDRESS_H_

#include <utils/copyable.h>
#include <utils/StringPiece.h>

#include <netinet/in.h>

namespace dbdky
{
namespace port
{
    class InetAddress : public dbdky::copyable
    {
    public:
        explicit InetAddress(uint16_t port);

        InetAddress(const StringPiece& ip, uint16_t port);
 
        InetAddress(const struct sockaddr_in& addr)
            : addr_(addr)
        {}

        string toIp() const;
        string toIpPort() const;
        string toHostPort() const __attribute__((deprecated))
        {
            return toIpPort();
        }

        const struct sockaddr_in& getSockAddrInet() const
        {
            return addr_;
        }

        void setSockAddrInet(const struct sockaddr_in& addr)
        {
            addr_ = addr;
        }

        uint32_t ipNetEndian() const
        {
            return addr_.sin_addr.s_addr;
        }

        uint16_t portNetEndian() const
        {
            return addr_.sin_port;
        }
    private:
        struct sockaddr_in addr_;
    };
}
}


#endif
