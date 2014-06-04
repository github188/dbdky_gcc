#ifndef __PDU_H_
#define __PDU_H_

#include <port/Buffer.h>
#include <utils/copyable.h>


using namespace dbdky;
using namespace dbdky::port;

namespace dbdky
{
namespace gcc
{
class pdu : dbdky::copyable
{
public:
    enum ProtocolType {kKemandga=0,
        kModbus,
        kTmpwia,
        kWeishuimidu,
        kZfsf6,
        kHdsf6,
        kLamodbus,
        kOmas,
        kVaisala,
        kYsp,
        kUnknown};
    
    pdu(Buffer& buf, ProtocolType ptl=kUnknown);
    ~pdu();

    ProtocolType getPduType() const;
    Buffer& getBuffer();

private:
    Buffer buf_;
    ProtocolType type_;
};
}
}


#endif    //__PDU_H_
