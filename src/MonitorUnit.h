#ifndef __MONITORUNIT_H_
#define __MONITORUNIT_H_

#include "MeasurePoint.h"

#include <string>
#include <map>
#include <boost/shared_ptr.hpp>

using namespace std;

class SerialPort;

namespace dbdky
{
namespace gcc
{
class MonitorUnit
{
public:
	MonitorUnit(SerialPort* port, string name, int interval, string protocolname, string mac,
		string manufacturer, string cycleid, string ytime);
	void onIntervalTimer();

	void insertMeasurePoint(string deviceid, string IEDName, string id, string checktime="");
	MeasurePoint* getMeasurePointByDeviceid(string deviceid);
private:
	string name_;
	int interval_;
	string protocolname_;
	SerialPort* port_;
	string manufacturer_;
	string cycleid_;
	string mac_;
	string ytime_;

	map<string, boost::shared_ptr<MeasurePoint> > measurePointList_;

};
}
}


#endif    //__MONITORUNIT_H_