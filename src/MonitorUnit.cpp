#include "MonitorUnit.h"
#include <utility>

#include <utils/Logging.h>

using namespace std;

namespace dbdky
{
namespace gcc
{
	MonitorUnit::MonitorUnit(string name, string interval, string protocolname, string mac,
		string manufacturer, string cycleid, string ytime)
      : name_(name),
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

	void MonitorUnit::dumpInfo() const
	{
		LOG_INFO << "****************MonitorUnit: " << name_ << "**************";
	 	LOG_INFO << "INTERVAL: " << interval_;
	 	LOG_INFO << "PROTOCOLNAME: " << protocolname_;
	 	LOG_INFO << "MANUFACTURER: " << manufacturer_;
	 	LOG_INFO << "CYCLEID: " << cycleid_;
	 	LOG_INFO << "MAC: " << mac_;
	 	LOG_INFO << "YTIME: " << ytime_;
	 	LOG_INFO << "++++++MeasurePoints++++++++++++++++++++++++++++++++++++";
	 	map<string, boost::shared_ptr<MeasurePoint> >::const_iterator itr;
	 	for (itr = measurePointList_.begin(); itr != measurePointList_.end(); itr++)
	 	{
	 		itr->second->dumpInfo();
	 	}
	}
}
}
	