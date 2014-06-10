#include "MeasurePoint.h"


namespace dbdky
{
namespace gcc
{
	MeasurePoint::MeasurePoint(string deviceid, string IEDName, string id, string checktime)
	  : deviceid_(deviceid),
	    IEDName_(IEDName),
	    id_(id),
	    checktime_(checktime)
	{

	}

	MeasurePoint::~MeasurePoint()
	{

	}
}
}