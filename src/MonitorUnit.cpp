#include "MonitorUnit.h"
#include <utility>

using namespace std;

namespace dbdky
{
namespace gcc
{
	MonitorUnit::MonitorUnit(SerialPort* port, string name, int interval, string protocolname, string mac,
		string manufacturer, string cycleid, string ytime)
      : port_(port),
        name_(name),
        interval_(interval),
        protocolname_(protocolname),
        mac_(mac),
        manufacturer_(manufacturer),
        cycleid_(cycleid),
        ytime_(ytime)
	{

	}

	void MonitorUnit::onIntervalTimer()
	{
		//TODO:
	}


	void MonitorUnit::insertMeasurePoint(string deviceid, string IEDName, string id, string checktime)
	{
		if (deviceid.empty())
		{
			return;
		}

		map<string, boost::shared_ptr<MeasurePoint> >::iterator itr;
		itr = measurePointList_.find(deviceid);
		if (measurePointList_.end() != itr)
		{
			return;
		}

		boost::shared_ptr<MeasurePoint> point(new MeasurePoint(deviceid, IEDName, id, checktime));
		measurePointList_.insert(make_pair<string, boost::shared_ptr<MeasurePoint> >(deviceid, point));

	}

	MeasurePoint* MonitorUnit::getMeasurePointByDeviceid(string deviceid)
	{
		if (deviceid.empty())
		{
			return NULL;
		}

        map<string, boost::shared_ptr<MeasurePoint> >::const_iterator itr;
        itr = measurePointList_.find(deviceid);
        if (measurePointList_.end() != itr)
        {
        	return itr->second.get();
        }

        return NULL;
	}
}
}
	