#ifndef __CODECBASE_H_
#define __CODECBASE_H_

#include <string>
#include <cstring>
#include <map>

#define IEC61850MODELLEN 128
#define MAXITEMSIZE 50
#define TYPEINTEGER 0
#define TYPEREAL 1
#define TYPECHAR 2
#define TYPEUTC 3

namespace dbdky
{
namespace gcc
{
   struct DB_INSERT_DATATYPE
   {
     char repID[IEC61850MODELLEN];
     char lnInstArray[MAXITEMSIZE][IEC61850MODELLEN];
     int  lnIDArraySize;
     char dataArray[MAXITEMSIZE][IEC61850MODELLEN];
     int  typeArray[IEC61850MODELLEN];
     char fieldArray[MAXITEMSIZE][IEC61850MODELLEN];
     int  paramSize;
   }; 
   enum{
      PARSE_SUCCESS = 0,
      PARSE_ERROR_LENGTH_SHORT = 1,
      PARSE_ERROR_OTHER = 3  
    };

class CodecBase
{
public:
	//Set Year&month
	virtual int makeYearMonthCmd( unsigned char id, unsigned char * codedata, int & len) = 0;
	//Set Day&hour
	virtual int makeDayHourCmd( unsigned char id, unsigned char * codedata, int & len) = 0;
	//Set Min&sec
	virtual int makeMinSecCmd( unsigned char id, unsigned char * codedata, int & len) = 0;
	//Not used
	virtual int makeThetimeCmd( unsigned char id, unsigned char * codedata, int & len ) = 0;
    // protocol code from parameter to code buffer
	virtual int makeQueryCmd( unsigned char id /* mac id 'Hex' Value*/,unsigned char *codedata,int &len ) = 0;
	// protocol decode from protocol response data to <parameter,value> pair list
	virtual int parser(const unsigned char *buffer,const int len,/* RS485 return data*/ void* out ) = 0;
	// config interface
	virtual int setConfig(const std::string lnInst,const std::map<std::string,float> & params_prop) = 0;
};
}
}
#endif
