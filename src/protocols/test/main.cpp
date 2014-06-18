//#include "CodescMMXN.h"
#include <stdio.h>
#include <dlfcn.h>
#include "CodecMMXN.h"
#include "CodecZSAR.h"

typedef dbdky::gcc::CodecBase* ( *GetInstanceFuncPtr ) ();
GetInstanceFuncPtr g_InstancePtr = NULL;

#if 1
int main(int argc, char** argv)
{
    return 0;
}
#else
int main(int argc ,char* argv[])
{
  printf("\n\n");
  printf("========================================\n");
  printf("| MMXN Test beginning ... ... ... ... ...\n");
  void * hGetInstanceHandle = dlopen("/home/lndk/work/svn_branch_0616/libMMXN.so",RTLD_NOW);
  if( NULL == hGetInstanceHandle )
  {
    printf("| Can't load libMMXN.so \n");    
    getchar();
    return 1;
  } 
  g_InstancePtr = ( GetInstanceFuncPtr ) dlsym(hGetInstanceHandle,"getInstance");
  if( NULL == g_InstancePtr )
  {
     printf("| %s\n",dlerror());
     return 1;
   }
   dbdky::gcc::CodecMMXN * instance = 
    dynamic_cast<dbdky::gcc::CodecMMXN*>( (dbdky::gcc::CodecBase *)g_InstancePtr() ); 
   if( NULL == instance )
   {
     printf("| Can't get Instance from the interface !\n");
     getchar();
     return 1;  
   }
   printf("| Testing interface of [ makeQueryCmd ] \n");

   int len = 14;
   unsigned char id = 0x01;
   unsigned char codedata[14];
   if( !instance->makeQueryCmd( id, codedata,len )  )
      printf("| Can't get QueryCmd for device id 0x07");
   else
   {
       printf("| ");
       for(int i = 0; i < len ;i++)
          printf("%02X ",codedata[i]);
       printf("\n");     
   }

   id = 0x0F;
   if( !instance->makeQueryCmd( id , codedata, len ) )
      printf("| Can't get QueryCmd for device id 0x0F");
   else
   {
       printf("| ");
       for(int i = 0; i < len ;i++)
          printf("%02X ",codedata[i]);
       printf("\n");     
   }
  printf("| Interface makeQueryCmd finished! \n");

  printf("| \n");
  printf("| Begin to make time . \n");
  id = 0x07;
  len = 14;
  printf("| Set Year-mon-day-hour-min-sec \n");
  if( !instance->makeThetimeCmd(id, codedata, len)  )
      printf("| Can't get makeYearMonthCmd for device id 0x07");
   else
   {
       printf("| ");
       for(int i = 0; i < len ;i++)
          printf("%02X ",codedata[i]);
       printf("\n");     
   }
  printf("| Set Year-mon   Day-hour Min-sec \n");
  len = 8;
  // Make the time
   if( !instance->makeYearMonthCmd(id, codedata, len)  )
      printf("| Can't get makeYearMonthCmd for device id 0x07");
   else
   {
       printf("| ");
       for(int i = 0; i < len ;i++)
          printf("%02X ",codedata[i]);
       printf("\n");     
   }
  
   len = 8;
   if( !instance->makeDayHourCmd(id, codedata, len)  )
      printf("Can't get makeDayHourCmd for device id 0x07");
   else
   {
       printf("| ");
       for(int i = 0; i < len ;i++)
          printf("%02X ",codedata[i]);
       printf("\n");     
   }

   len = 8;
   if( !instance->makeMinSecCmd(id, codedata, len)  )
      printf("| Can't get makeMinSecCmd for device id 0x07");
   else
   {
       printf("| ");
       for(int i = 0; i < len ;i++)
          printf("%02X ",codedata[i]);
       printf("\n");     
   }
  //process the response data 
   printf("| Make time finished !\n");
   printf("| \n");
   printf("| Begin to parse data! \n");
   
   std::map<std::string, float> paraPair;
   paraPair["Amp"] = 0.1;

   instance->setConfig( paraPair ); 

   //Send message : 01 03 00 10 00 04 45 CC
   //Response     : 01 03 08 10 09 30 12 35 38 00 05 FE 2A
   unsigned char buffer[512] = {0x01,0x04,0x08,0x0E,0x06,0x10,0x10,0x1E,0x2A,0x00,0x05,0xFE,0x2A};
   printf("| Response data is : ");
   for(int i = 0;i < 13;i++)
      printf("%02X ",buffer[i]);
   printf("\n");
   //Set the buffer len
   len = 13 ;//sizeof(DB_INSERT_DATATYPE);
   dbdky::gcc::DB_INSERT_DATATYPE out;
   
   printf("| Testing MMXN7 parser ! \n");
   if( !instance->parser( buffer,len,"MMXN7",(void *) &out))
   {
     printf("Parse failed!\n");
   }else
   {
      printf("| Parser SUCCESS! \n");

      dbdky::gcc::DB_INSERT_DATATYPE * p = (dbdky::gcc::DB_INSERT_DATATYPE *) (&out); 
      printf("| ReportID = [ %s ] \n" , p->repID );      
      for( int i = 0;i < p->paramSize;i++ )
      {
         printf("| ParamName:    [ %s ]\n",p->fieldArray[i] );
	 printf("| ParamValue:   [ %s ]\n",p->dataArray[i]);
       }
    }
 
  printf("| =======================================\n");
  printf("| MMXN Test finished! ... ... ... ... ...");
  printf("\n\n\n\n");



  printf("| =======================================\n");
  printf("| ZSAR Test beginning ... ... ... ... ...\n");
  hGetInstanceHandle = dlopen("/home/lndk/work/svn_branch_0616/libZSAR.so",RTLD_NOW);
  if( NULL == hGetInstanceHandle )
  {
    printf("| Can't load libZSAR.so \n");    
    getchar();
    return 1;
  } 
  g_InstancePtr = ( GetInstanceFuncPtr ) dlsym(hGetInstanceHandle,"getInstance");
  if( NULL == g_InstancePtr )
  {
     printf("| %s\n",dlerror());
     return 1;
   }
   dbdky::gcc::CodecZSAR * instance_zsar = dynamic_cast<dbdky::gcc::CodecZSAR*>( (dbdky::gcc::CodecBase *)g_InstancePtr() ); 
   if( NULL == instance )
   {
     printf("| Can't get Instance from the interface !\n");
     getchar();
     return 1;  
   }
   printf("| Testing interface of [ makeQueryCmd ] \n");

   len = 8;
   id = 0x07;
   if( !instance_zsar->makeQueryCmd( id, codedata,len )  )
      printf("| Can't get QueryCmd for device id 0x07");
   else
   {
       printf("| ");
       for(int i = 0; i < len ;i++)
          printf("%02X ",codedata[i]);
       printf("\n");     
   }

   id = 0x0D;
   if( !instance_zsar->makeQueryCmd( id , codedata, len ) )
      printf("| Can't get QueryCmd for device id 0x0F");
   else
   {
       printf("| ");
       for(int i = 0; i < len ;i++)
          printf("%02X ",codedata[i]);
       printf("\n");     
   }
  printf("| Interface makeQueryCmd finished! \n");

  printf("| \n");
  printf("| Begin to make time . \n");
  id = 0x07;
  // Make the time
   if( !instance_zsar->makeYearMonthCmd(id, codedata, len)  )
      printf("| Can't get makeYearMonthCmd for device id 0x07");
   else
   {
       printf("| ");
       for(int i = 0; i < len ;i++)
          printf("%02X ",codedata[i]);
       printf("\n");     
   }
  
  
   if( !instance_zsar->makeDayHourCmd(id, codedata, len)  )
      printf("| Can't get makeDayHourCmd for device id 0x07");
   else
   {
       printf("| ");
       for(int i = 0; i < len ;i++)
          printf("%02X ",codedata[i]);
       printf("\n");     
   }

 
   if( !instance_zsar->makeMinSecCmd(id, codedata, len)  )
      printf("| Can't get makeMinSecCmd for device id 0x07");
   else
   {
       printf("| ");
       for(int i = 0; i < len ;i++)
          printf("%02X ",codedata[i]);
       printf("\n");     
   }
   printf("| Make time finished !\n");
   printf("| \n");
   printf("| Begin to parse data! \n");
     
   unsigned char buffer_zsar[512] = {0x01,0x04,0x08,0x10,0x09,0x30,0x12,0x35,0x38,0x00,0x05,0xFE,0x2A};
   printf("| Response data is : ");
   for(int i = 0;i < 13;i++)
      printf("%02X ",buffer[i]);
   printf("\n");
   //Set the buffer len
   len = 13 ;//sizeof(DB_INSERT_DATATYPE);
   

   printf("| Testing ZSAR1 parser ! \n");
   if( !instance_zsar->parser( buffer_zsar,len,"ZSAR1",(void *) &out))
   {
     printf("| Parse failed!\n");
   }else
   {
      printf("| Parser SUCCESS! \n");

      dbdky::gcc::DB_INSERT_DATATYPE * p = (dbdky::gcc::DB_INSERT_DATATYPE *) (&out); 
      printf("| ReportID = [ %s ] \n" , p->repID );      
      for( int i = 0;i < p->paramSize;i++ )
      {
         printf("| ParamName:    [ %s ]\n",p->fieldArray[i] );
	 printf("| ParamValue:   [ %s ]\n",p->dataArray[i]);
       }
    }
 

  printf("| =======================================\n");
  printf("| MMXN Test finished! ... ... ... ... ...");
  printf("| \n\n\n");

  printf("***************************************\n");
  printf("Test suite run successfully ! \n");
  return 0;
}

#endif