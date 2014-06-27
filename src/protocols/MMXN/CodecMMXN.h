#ifndef  CODEC_MMXN_
#define  CODEC_MMXN_

#include "CodecBase.h"

namespace dbdky
{
namespace gcc
{
class CodecMMXN: public CodecBase
{
public:

	int makeYearMonthCmd( unsigned char id, unsigned char * codedata, int & len);

	int makeDayHourCmd( unsigned char id, unsigned char * codedata, int & len);

	int makeMinSecCmd( unsigned char id, unsigned char * codedata, int & len);

	int makeThetimeCmd( unsigned char id, unsigned char * codedata, int & len );

	int makeQueryCmd( unsigned char id ,unsigned char * codedata,int & len );

	int parser( const unsigned char *buffer,const int len, void * out );

	int setConfig(const std::string lnInst,const std::map<std::string,float> & params_prop);
	
public:
    std::map<std::string,float> m_param_precisions;
    std::string lnInst_;
};
}
}

#endif
