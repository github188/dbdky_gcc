#include "param.h"


#include <utils/Logging.h>

namespace dbdky
{
namespace gcc
{
    void Param::dumpInfo() const
    {
    	LOG_INFO << "***********Param: " << name_ << "*******************";
		LOG_INFO << "DESC: " << desc_;
	 	LOG_INFO << "UNIT: " << unit_;
	 	LOG_INFO << "PRECISION: " << precision_;
	 	LOG_INFO << "RANGE: " << range_;
	 	LOG_INFO << "FILTER: " << filter_;
	 	LOG_INFO << "ADDRESSLEN: " << addresslen_;
	 	LOG_INFO << "ADDRESS: " << address_;
	 	LOG_INFO << "REGISTERNUM: " << registernum_;
    }
}
}