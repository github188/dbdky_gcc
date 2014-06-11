#ifndef __PARAM_H_
#define __PARAM_H_

#include <string>

using namespace std;

namespace dbdky
{
namespace gcc
{

class MeasurePoint;

class Param
{
public:
	friend class MeasurePoint;
	Param(string name, string desc="", string unit="", string precision="", 
		string range="", string filter="",
		string addresslen="", string address="", string registernum="")
	  : name_(name),
	    desc_(desc),
	    unit_(unit),
	    precision_(precision),
	    range_(range),
	    filter_(filter),
	    addresslen_(addresslen),
	    address_(address),
	    registernum_(registernum)
	    {

	    }

	~Param()
	{

	}
private:
	string name_;
	string desc_;
	string unit_;
	string precision_;
	string range_;
	string filter_;
	string addresslen_;
	string address_;
	string registernum_;
};
}
}

#endif    //__PARAM_H_