#ifndef __MEASUREPOINT_H_
#define __MEASUREPOINT_H_

#include <string>
#include <map>
#include <boost/shared_ptr.hpp>

#include "param.h"

using namespace std;

class MonitorUnit;

namespace dbdky
{
namespace gcc
{
class MeasurePoint
{
public:
	MeasurePoint(string deviceid, string IEDName, string id, string checktime);
	~MeasurePoint();

	Param* getParamByName(string name);
	void insertParam(string name, string desc="", string unit="", string precision="", 
		string range="", string filter="",
		string addresslen="", string address="", string registernum="");
	void insertParam(Param& param);

	void dumpInfo() const;

	map<string, boost::shared_ptr<Param> >& getParamList()
	{
		return paramList_;
	}
	
private:
	string deviceid_;
	string IEDName_;
	string id_;
	string checktime_;
	map<string, boost::shared_ptr<Param> > paramList_;

};
}
}


#endif    //__MEASUREPOINT_H_