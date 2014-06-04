#ifndef __CODECFACTORY_H_
#define __CODECFACTORY_H_

#include "CodecBase.h"
#include "pdu.h"

namespace dbdky
{
namespace gcc
{
class CodecFactory
{
public:
	static CodecFactory* getInstance();
	CodecFactory(){}
	CodecBase* getCodec(pdu::ProtocolType type = pdu::kUnknown);

private:
	static CodecFactory* instance_;

};
}
}

#endif    //__CODECFACTORY_H_