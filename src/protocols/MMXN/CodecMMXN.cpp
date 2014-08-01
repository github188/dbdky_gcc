#include "crc.h"
#include "CodecMMXN.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define MMXN_MKTIME_CMD_LEN 	  14
#define MMXN_QUERY_CMD_LEN  	  8
#define MMXN_SET_TIME_REG_CMD_LEN 8
#define MMXN_DATA_MAX_LEN   	  128
#define MMXN_CRC_BYTE_LEN         2

#ifdef __cplusplus
extern "C"
{
#endif
dbdky::gcc::CodecBase* getInstance()
{
    return new dbdky::gcc::CodecMMXN();
}
#ifdef __cplusplus
}
#endif

namespace dbdky
{
namespace gcc
{

    typedef struct{
     unsigned char addr;
     unsigned char code;
     unsigned char length;
     unsigned char dat[ MMXN_DATA_MAX_LEN ];
     unsigned char crc16[ MMXN_CRC_BYTE_LEN ];
     } RECMMXN, * pRECMMXN;

    unsigned short CRC16( unsigned char * puchMsg, unsigned short usDataLen )
    {
      unsigned short  uIndex;
      unsigned char uchCRCHi; //= 0xFF; 
      unsigned char uchCRCLo; // = 0xFF; 
    
      uchCRCHi = 0xFF;
      uchCRCLo = 0xFF;
      while (usDataLen--)
      {
         uIndex   = uchCRCHi ^ *puchMsg++ ; 
         uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex];
         uchCRCLo = auchCRCLo[uIndex] ;
      }
      
       return (uchCRCHi << 8 | uchCRCLo) ; 
    }
	int CodecMMXN::setConfig(const std::string lnInst,const std::map<std::string,float> & params_prop)
	{
		for(std::map<std::string,float>::const_iterator it = params_prop.begin();it != params_prop.end();it++ )
		{
			m_param_precisions[it->first] = it->second;
		}
		lnInst_ = lnInst;
	}
    int CodecMMXN::makeYearMonthCmd( unsigned char id, unsigned char * codedata, int & len)
	{
	       //get current time
	       if( len < MMXN_SET_TIME_REG_CMD_LEN )
	    	 return 0;
	       time_t tCur;
	       unsigned short uCRC = 0;
	       tCur = time( NULL );
	       struct tm *tmCur;
	       tmCur = localtime( & tCur );

	       unsigned short year  = ( tmCur->tm_year + 1900 - 2000 );
	       unsigned short month = tmCur->tm_mon + 1;

	       unsigned short day   = tmCur->tm_mday;
	       unsigned short hour  = tmCur->tm_hour;

	       unsigned short min   = tmCur->tm_min;
	       unsigned short sec   = tmCur->tm_sec;

	       //06 00 10 year mon mac || 06 00 11 day hour mac || 06 00 12 min sec mac
	       codedata[0] = id;
	       codedata[1] = 0x06;

	       //set year & month
	       codedata[2] = 0x00; //register start high addr
	       codedata[3] = 0x05; //register start low addr
	       codedata[4] = year; //set year  register high BYTE
	       codedata[5] = month;//set month register low BYTE

	       uCRC   = CRC16( codedata, 6 );

	       codedata[6] = (unsigned char)(uCRC >> 8);
	       codedata[7] = (unsigned char)(uCRC);

	       len    = MMXN_SET_TIME_REG_CMD_LEN;

	       return 1;

	}

	int CodecMMXN::makeDayHourCmd( unsigned char id, unsigned char * codedata, int & len)
	{
	       //get current time
	       if( len < MMXN_SET_TIME_REG_CMD_LEN )
	    	 return 0;
	       time_t tCur;
	       unsigned short uCRC = 0;
	       tCur = time( NULL );
	       struct tm *tmCur;
	       tmCur = localtime( & tCur );

	       unsigned short year  = ( tmCur->tm_year + 1900 - 2000 );
	       unsigned short month = tmCur->tm_mon + 1;

	       unsigned short day   = tmCur->tm_mday;
	       unsigned short hour  = tmCur->tm_hour;

	       unsigned short min   = tmCur->tm_min;
	       unsigned short sec   = tmCur->tm_sec;

	       //06 00 10 year mon mac || 06 00 11 day hour mac || 06 00 12 min sec mac
	       codedata[0] = id;
	       codedata[1] = 0x06;

	       //set day & hour
	       codedata[2] = 0x00;
	       codedata[3] = 0x06;
	       codedata[4] = day;
	       codedata[5] = hour;

	       uCRC   = CRC16( codedata, 6 );
	       codedata[6] = (unsigned char)(uCRC >> 8);
	       codedata[7] = (unsigned char)(uCRC);

	       len    = MMXN_SET_TIME_REG_CMD_LEN;

		   return 1;

	}

	int CodecMMXN::makeMinSecCmd( unsigned char id, unsigned char * codedata, int & len)
	{
	       //get current time
	       if( len < MMXN_SET_TIME_REG_CMD_LEN )
	    	 return 0;
	       time_t tCur;
	       unsigned short uCRC = 0;
	       tCur = time( NULL );
	       struct tm *tmCur;
	       tmCur = localtime( & tCur );

	       unsigned short year  = ( tmCur->tm_year + 1900 - 2000 );
	       unsigned short month = tmCur->tm_mon + 1;

	       unsigned short day   = tmCur->tm_mday;
	       unsigned short hour  = tmCur->tm_hour;

	       unsigned short min   = tmCur->tm_min;
	       unsigned short sec   = tmCur->tm_sec;

	       //06 00 10 year mon mac || 06 00 11 day hour mac || 06 00 12 min sec mac
	       codedata[0] = id;
	       codedata[1] = 0x06;

	       //set minutes & seconds
	       codedata[2] = 0x00;
	       codedata[3] = 0x07;
	       codedata[4] = min;
	       codedata[5] = sec;

	       uCRC   = CRC16( codedata , 6 );

	       codedata[6] = (unsigned char)(uCRC >> 8);
	       codedata[7] = (unsigned char)(uCRC);

	       len    = MMXN_SET_TIME_REG_CMD_LEN;
	       return 1;

	}
    int CodecMMXN::makeThetimeCmd( unsigned char id, unsigned char *out, int & len)
    {
       //get current time 
       if( len < MMXN_MKTIME_CMD_LEN )
    	 return 0;
       time_t tCur;
       unsigned short uCRC = 0;
       tCur = time( NULL ) ;
       struct tm * tmCur;
       tmCur = localtime(  &tCur );

       unsigned short year  = ( tmCur->tm_year + 1900 - 2000 );
       unsigned short month = tmCur->tm_mon + 1;

       unsigned short day   = tmCur->tm_mday;
       unsigned short hour  = tmCur->tm_hour;

       unsigned short min   = tmCur->tm_min;
       unsigned short sec   = tmCur->tm_sec;

       //06 00 10 year mon mac || 06 00 11 day hour mac || 06 00 12 min sec mac
       out[0]  = id;
       out[1]  = 0x10;

       out[2]  = 0x00; //register start high addr
       out[3]  = 0x05; //register start low addr
       out[4]  = 0x00;
       out[5]  = 0x03;
       out[6]  = year; //set year  register high BYTE
       out[7]  = month;//set month register low BYTE
       out[8]  = day;
       out[9]  = hour;
       out[10] = min;
       out[11] = sec;

       uCRC    = CRC16( out, MMXN_MKTIME_CMD_LEN - 2 );
       out[12] = (unsigned char) ( uCRC >> 8);
       out[13] = (unsigned char) ( uCRC );    

       len     = MMXN_MKTIME_CMD_LEN;  
       return    1;
    }

    int CodecMMXN::makeQueryCmd( unsigned char id, unsigned char *out,int & len)
    {
       if(len < MMXN_QUERY_CMD_LEN )
          return 0;
       
       //get the command
       out[0] = id;    //Hex
       out[1] = 0x04;  //function code query data
       out[2] = 0x00;
       out[3] = 0x21;
       out[4] = 0x00;
       out[5] = 0x08;
       unsigned short uCRC;
       uCRC   = CRC16( out, 6 );
       out[6] = (unsigned char) ( uCRC >> 8);
       out[7] = (unsigned char) ( uCRC );    

       len    = MMXN_QUERY_CMD_LEN;
       return 1;	
    }

    int CodecMMXN::parser( const unsigned char * buffer, const int len,void * out)
    {
       //filter data
       //Merge from Yandan, begin
       if (len < 21)
       {
           return PARSE_ERROR_LENGTH_SHORT;
       }
       //Merge from Yandan, end

       pRECMMXN pRs485Data;
       unsigned char data[MMXN_DATA_MAX_LEN];
       char strTmDat[32];
       struct tm tmp;
       int nPDUIndex = 0;
       pRs485Data = ( pRECMMXN ) ( const_cast<unsigned char*>(buffer) );
       if(NULL == pRs485Data)
	return 0;
       if( pRs485Data->length > MMXN_DATA_MAX_LEN )
          return 0;

      //Merge from Yandan, begin
       if (pRs485Data->length > MMXN_DATA_MAX_LEN)
        {
            return PARSE_ERROR_OTHER;
        }
      //Merge from Yandan, end
       memcpy( data, pRs485Data->dat,pRs485Data->length );
       
       //get parameter  the order rely on xml config file
       memset(&tmp,0,sizeof(tm));
       
       sprintf(strTmDat,"20%02d",data[nPDUIndex++]);
       tmp.tm_year = atoi(strTmDat) - 1900;
       
       sprintf(strTmDat,"%d",data[nPDUIndex++]);
       tmp.tm_mon = atoi(strTmDat) - 1;

       sprintf(strTmDat,"%d",data[nPDUIndex++]);
       tmp.tm_mday = atoi(strTmDat);

       sprintf(strTmDat,"%d",data[nPDUIndex++]);
       tmp.tm_hour = atoi(strTmDat);
   
       sprintf(strTmDat,"%d",data[nPDUIndex++]);
       tmp.tm_min = atoi(strTmDat);

       sprintf(strTmDat,"%d",data[nPDUIndex++]);
       tmp.tm_sec = atoi(strTmDat);
       //get IEC 61850 format data

       time_t smpTm64;
       char smpTm[64];
       smpTm64 = mktime(&tmp);
     
       smpTm64 += ( 8 * 60 * 60 );
   
       sprintf(smpTm,"%d",(int)smpTm64);
       
       unsigned short ampData = 0;
       float precision = 1.0;
       //Change to dest value       
       if( m_param_precisions.find( "Amp" ) != m_param_precisions.end() )
       {
	    precision =  m_param_precisions["Amp"];
	}
       
       float fAmp = 0.0; 

       //For Maojun revision
#ifndef MMXN_SETP
       ampData              =  data[nPDUIndex++];
       ampData              =  ( ampData << 8 );
       ampData             +=  data[nPDUIndex++];
       
       if( ampData > 1000 )
	  fAmp = ampData;
       else
          fAmp = (ampData * precision);       
#else  //For Step algorithm
      float value = 0.0;
      ampData               =  data[nPDUIndex++];
      unsigned char tag     =  ampData;
      tag                   =  ( tag & (0xC0) ) >> 6;
      ampData		    = ( ampData& (0x3F) ) << 8;
      ampData              +=  data[nPDUIndex++];      

      if( 0 == tag)
          value = ampData  * 0.01;
      else if( 1 == tag )
	  value = ampData  * 0.1;
      else if( 2 == tag )
          value = ampData * 1.0;

      fAmp = value ; 
#endif 
       
       char strAmp[128];
       sprintf(strAmp,"%f",fAmp);
           
       DB_INSERT_DATATYPE *pInsertRecord = (struct DB_INSERT_DATATYPE *) out; 
       strcpy(pInsertRecord->lnInstArray[0] ,lnInst_.c_str());
       strcpy(pInsertRecord->repID,"COMDATA/");
       strcat(pInsertRecord->repID,lnInst_.c_str());

       pInsertRecord->lnIDArraySize = 1;
       pInsertRecord->paramSize = 2;

       strcpy(pInsertRecord->dataArray[0],smpTm);
       strcpy(pInsertRecord->dataArray[1],strAmp);

       pInsertRecord->typeArray[0] = TYPEUTC;
       pInsertRecord->typeArray[1] = TYPEREAL;

       strcpy( pInsertRecord->fieldArray[0], "SmpTm" );
       strcpy( pInsertRecord->fieldArray[1], "Amp" );

       return PARSE_SUCCESS;
    }
  }
}
