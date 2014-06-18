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

	int parser( const unsigned char *buffer,const int len, char * lnInst, void * out );

	int setConfig(const std::map<std::string,float> & params_prop);
	
public:
    std::map<std::string,float> m_param_precisions;
};
}
}

#endif
