#ifndef __CODECFACTORY_H_
#define __CODECFACTORY_H_

#include "CodecBase.h"
#include "pdu.h"

#include <string>

using namespace std;

namespace dbdky
{
namespace gcc
{
class CodecFactory
{
public:
	static CodecFactory* getInstance(string codecpath="");
	CodecFactory(string codecpath)
	  : codecpath_(codecpath)
	  {

	  }
	CodecBase* getCodec(string protocolname);

private:
	static CodecFactory* instance_;
    string codecpath_;
};
}
}

#endif    //__CODECFACTORY_H_