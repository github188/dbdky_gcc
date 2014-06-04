#include "pdu.h"

namespace dbdky
{
namespace gcc
{
    pdu::pdu(Buffer& buf, ProtocolType ptl)
        : buf_(buf),
          type_(ptl)
    {
    }
  
    pdu::~pdu()
    {
    }

    pdu::ProtocolType pdu::getPduType() const
    {
        return type_;
    }
    
    Buffer& pdu::getBuffer()
    {
        return buf_;
    }
}
}
