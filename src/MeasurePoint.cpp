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

	Param* MeasurePoint::getParamByName(string name)
	{
		if (name.empty())
		{
			return NULL;
		}

		map<string, boost::shared_ptr<Param> >::const_iterator itr;
		itr = paramList_.find(name);
		if (paramList_.end() != itr)
		{
			return itr->second.get();
		}

		return NULL;
	}

	void MeasurePoint::insertParam(string name, string desc, string unit, string precision, 
		string range, string filter,
		string addresslen, string address, string registernum)
	{
		if (name.empty())
		{
			return;
		}

		map<string, boost::shared_ptr<Param> >::const_iterator itr;
		itr = paramList_.find(name);
		if (paramList_.end() == itr)
		{
			boost::shared_ptr<Param> param(new Param(name, desc, unit, precision, 
				range, filter, addresslen, address, registernum));
			paramList_.insert(make_pair<string, boost::shared_ptr<Param> >(name, param));
		}
	}

	void insertParam(Param& param)
	{
		insertParam(param.name_, param.desc_, param.unit_, param.precision_,
			param.range_, param.filter_, param.addresslen_, param.address_, param.registernum_);
	}
}
}