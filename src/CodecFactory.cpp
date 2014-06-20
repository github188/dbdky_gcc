#include "CodecFactory.h"

#include <stdio.h>
#include <dlfcn.h>

namespace dbdky
{
namespace gcc
{
CodecFactory* CodecFactory::instance_ = NULL;

CodecFactory* CodecFactory::getInstance(string codecpath)
{
	if (instance_ == NULL)
	{
		instance_ = new CodecFactory(codecpath);
	}

	return instance_;
}

CodecBase* CodecFactory::getCodec(string protocolname)
{
	//TODO:
	return NULL;
}
}
}