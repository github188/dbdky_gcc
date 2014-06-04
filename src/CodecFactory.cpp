#include "CodecFactory.h"

namespace dbdky
{
namespace gcc
{
CodecFactory* CodecFactory::instance_ = NULL;

CodecFactory* CodecFactory::getInstance()
{
	if (instance_ == NULL)
	{
		instance_ = new CodecFactory();
	}

	return instance_;
}

CodecBase* CodecFactory::getCodec(pdu::ProtocolType type)
{
	//TODO:
	return NULL;
}
}
}