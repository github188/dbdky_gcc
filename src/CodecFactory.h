#ifndef __CODECFACTORY_H_
#define __CODECFACTORY_H_

#include "CodecBase.h"
#include "pdu.h"

#include <string>
#include <map>
#include <boost/shared_ptr.hpp>


using namespace std;

namespace dbdky
{
namespace gcc
{
class CodecFactory
{
public:
	typedef dbdky::gcc::CodecBase* (*GetInstanceFuncPtr) ();
	
	static CodecFactory* getInstance();
	CodecBase* getCodec(string protocolname);

private:
	CodecFactory();
	static CodecFactory* instance_;
	map<string, boost::shared_ptr<CodecBase> > codecList_;
};
}
}

#endif    //__CODECFACTORY_H_