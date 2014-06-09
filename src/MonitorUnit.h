#ifndef __MONITORUNIT_H_
#define __MONITORUNIT_H_

#include "MeasurePoint.h"

#include <string>
#include <list>

using namespace std;

namespace dbdky
{
namespace gcc
{
class MonitorUnit
{
public:
	void onIntervalTimer();
private:
	string name_;
	int interval_;
	string protocolname_;
	string port_;
	string manufacture_;
	string cycleid_;
	string mac_;
	string ytime_;

	list<MeasurePoint> measurePointList_;
};
}
}


#endif    //__MONITORUNIT_H_