#include "crc.h"
#include "CodecZSAR.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif
dbdky::gcc::CodecBase* getInstance()
{
    return new dbdky::gcc::CodecZSAR();
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
         unsigned char dat[128];
         unsigned char crc16[2];
     } RECZSAR, * pRECZSAR;

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
	int CodecZSAR::setConfig(const std::string lnInst,const std::map<std::string,float> & params_prop)
	{
		for(std::map<std::string,float>::const_iterator it = params_prop.begin();it != params_prop.end();it++ )
		{
			m_param_precisions[it->first] = it->second;
		}
		lnInst_ = lnInst;
	}
	int CodecZSAR::makeYearMonthCmd( unsigned char id, unsigned char * codedata, int & len)
	{
	       //get current time
	       if( len < 8 )
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
	       len    = 8;

	       return 1;

	}

	int CodecZSAR::makeDayHourCmd( unsigned char id, unsigned char * codedata, int & len)
	{
	       //get current time
	       if( len < 8 )
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
	       len    = 8;

	       return 1;

	}

	int CodecZSAR::makeMinSecCmd( unsigned char id, unsigned char * codedata, int & len)
	{
	       //get current time
	       if( len < 8 )
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
	       len    = 8;
	       return 1;

	}

    int CodecZSAR::makeThetimeCmd( unsigned char id, unsigned char *out, int & len)
    {
       //get current time 
       if( len < 14 )
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
       out[0]  = id;
       out[1]  = 0x10;

       //set year & month
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

       uCRC    = CRC16( out, 12 );
       out[12] = (unsigned char) ( uCRC >> 8);
       out[13] = (unsigned char) ( uCRC );    
       len     = 14;
       return     1;
    }

    int CodecZSAR::makeQueryCmd( unsigned char id, unsigned char *out,int & len)
    {
       if(len <8 )
          return 0;
       //get the command
       out[0] = id;    //Hex
       out[1] = 0x04;  //function code query data
       out[2] = 0x00;
       out[3] = 0x20;
       out[4] = 0x00;
       out[5] = 0x0A;
       unsigned short uCRC;
       uCRC = CRC16( out, 6 );
       out[6] = (unsigned char) ( uCRC >> 8);
       out[7] = (unsigned char) ( uCRC );    

       len = 8;
       
       return 1;	
    }

    int CodecZSAR::parser( const unsigned char * buffer,const int len, void * out)
    {
       //filter data
       //Merge from Yandan, begin
       if (len < 25)
       {
           return PARSE_ERROR_LENGTH_SHORT;
       }       
              //CRC Check 
       unsigned short uCRC;
       unsigned char CRCOut[2];
       uCRC = CRC16( (unsigned char*)buffer, 23 );     
       CRCOut[0] = ( unsigned char )( uCRC >> 8);
       CRCOut[1] = ( unsigned char )( uCRC );
      // printf("CRC = %02X %02X\n",CRCOut[0],CRCOut[1]);

       if(!(( CRCOut[0] == buffer[23] ) && ( CRCOut[1] == buffer[24] )))
       {
           return PARSE_ERROR_OTHER;
       }
       //Merge from Yandan, end
       
       pRECZSAR pRs485Data = NULL;
       unsigned char data[128] = {0};
       char strTmDat[32] = {0};
       struct tm tmp = {0};
       int nPDUIndex = 0;
       pRs485Data = (pRECZSAR) (const_cast<unsigned char*>(buffer) );
       
	   if(NULL == pRs485Data)
	   {
	       return PARSE_ERROR_OTHER;
	   }
       if( pRs485Data->length > 128 )
	   {
           return PARSE_ERROR_OTHER;
	   }
       
	   //skip two bytes of warning flag
	   memcpy( data, pRs485Data->dat + 2, pRs485Data->length - 2 );

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

       time_t smpTm64 = 0;
       char strSmpTm[32] = {0};
       smpTm64 = mktime(&tmp);
       smpTm64 += ( 8 * 60 * 60 );
       sprintf(strSmpTm,"%d",(int)smpTm64);
       

       //(1)TotA : -----------------------------
       unsigned char totaData = data[nPDUIndex++];
       totaData               = ( totaData << 8 );
       totaData              += data[nPDUIndex++];
       
       float fTotAPrecision = 1.0;
       //Change to dest value       
       if( m_param_precisions.find( "TotA" ) != m_param_precisions.end() )
       {
	       fTotAPrecision =  m_param_precisions["TotA"];
       }
       float fTotA = 0.0; 
       if( totaData > 1000 )
	   {
	       fTotA = totaData;
	   }
       else
	   {
           fTotA = (totaData * fTotAPrecision);       
       }
       char strTotA[32] = {0};
       sprintf(strTotA,"%f",fTotA);

       //(2)RisA : -----------------------------
       unsigned char risaData = data[nPDUIndex++];
       risaData               = ( risaData << 8 );
       risaData              += data[nPDUIndex++];
       
       float fRisAprecision = 1.0;
       //Change to dest value       
       if( m_param_precisions.find( "RisA" ) != m_param_precisions.end() )
       {
	       fRisAprecision =  m_param_precisions["RisA"];
	   }
       float fRisA = 0.0; 
       if( risaData > 1000 )
	   {
	       fRisA = risaData;
	   }
       else
	   {
           fRisA = (risaData * fRisAprecision);
	   }       
       
       char strRisA[32] = {0};
       sprintf(strRisA,"%f",fRisA);
       
        //(3)OpCnt : -----------------------------
       unsigned char optcntData = data[nPDUIndex++];
       optcntData               = ( optcntData << 8 );
       optcntData              += data[nPDUIndex++];

       int nOpCnt = 0; 
       nOpCnt = (int)optcntData;       
       
       char strOptCnt[32] = {0};
       sprintf(strOptCnt,"%d",nOpCnt);

       //(4)LastOpTm : -----------------------------       
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

       time_t lastOpTm64 = 0;
       char strLastOpTm[32] = {0};
       lastOpTm64 = mktime(&tmp);
	   int nLastOpTm = (int)lastOpTm64;
       
	   if((int)lastOpTm64 == -1)
	   {
	       nLastOpTm = 0;
	   }
       sprintf(strLastOpTm,"%d",nLastOpTm);
       
       
       DB_INSERT_DATATYPE *pInsertRecord = (struct DB_INSERT_DATATYPE *) out; 
       strcpy(pInsertRecord->lnInstArray[ 0 ] ,lnInst_.c_str());
       
       strcpy(pInsertRecord->repID,"COMDATA/");
       strcat(pInsertRecord->repID,lnInst_.c_str());

       pInsertRecord->lnIDArraySize = 1;
       pInsertRecord->paramSize = 5;

       //filed
       strcpy(pInsertRecord->fieldArray[0] , "SmpTm");   
       strcpy(pInsertRecord->fieldArray[1] , "TotA");   
       strcpy(pInsertRecord->fieldArray[2] , "RisA"); 
       strcpy(pInsertRecord->fieldArray[3] , "OpCnt"); 
	   strcpy(pInsertRecord->fieldArray[4] , "LastOpTm");

       //type
       pInsertRecord->typeArray[0] = TYPEUTC;
       pInsertRecord->typeArray[1] = TYPEREAL;
       pInsertRecord->typeArray[2] = TYPEREAL;
       pInsertRecord->typeArray[3] = TYPEINTEGER;
       pInsertRecord->typeArray[4] = TYPEUTC;

	   //data
       strcpy(pInsertRecord->dataArray[0], strSmpTm);
       strcpy(pInsertRecord->dataArray[1], strTotA);
       strcpy(pInsertRecord->dataArray[2], strRisA);
       strcpy(pInsertRecord->dataArray[3], strOptCnt);
	   strcpy(pInsertRecord->dataArray[4], strLastOpTm);

     return PARSE_SUCCESS;
    }
}
}
