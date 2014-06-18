#ifndef  CODEC_ZSAR_
#define  CODEC_ZSAR_

#include "CodecBase.h"

namespace dbdky
{
namespace gcc
{
class CodecZSAR: public CodecBase
{
public:

	int makeYearMonthCmd( unsigned char id, unsigned char * codedata, int & len);

	int makeDayHourCmd( unsigned char id, unsigned char * codedata, int & len);

	int makeMinSecCmd( unsigned char id, unsigned char * codedata, int & len);

	int makeThetimeCmd( unsigned char id, unsigned char * codedata, int & len );

	int makeQueryCmd( unsigned char id ,unsigned char * codedata,int & len );

	int parser( const unsigned char *buffer, const int len,char * lnInst, void * out );

	int setConfig(const std::map<std::string,float> & params_prop);
	
private:
    std::map<std::string,float> m_param_precisions;

};
}
}

#endif
