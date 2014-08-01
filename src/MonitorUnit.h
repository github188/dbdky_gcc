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
	MonitorUnit(string name, string interval, string protocolname, string mac,
		string manufacturer, string cycleid, string ytime);
	void onIntervalTimer();

	void insertMeasurePoint(string deviceid, string IEDName, string id, string checktime="");
	MeasurePoint* getMeasurePointByDeviceid(string deviceid);
        
        //Added by Chen Hongquan, begin
        //Here comes a very ugly interface.
        //This interface is implemented based on the assumption that there
        //is only ONE measure point in a MonitorUnit.
        MeasurePoint* getFirstMeasurePoint()
        {
            if (measurePointList_.empty())
            {
                return NULL;
            }

            return measurePointList_.begin()->second.get();
     
        }
        //Added by Chen Hongquan ,end

	void dumpInfo() const;

	string getInterval() const
	{
		return interval_;
	}

	string getProtocolName() const
	{
		return protocolname_;
	}

	map<string, float> getParamConfigList() const;

	string getMac() const
	{
		return mac_;
	}

    string getName() const
    {
    	return name_;
    }
    
private:
	string name_;
	string interval_;
	string protocolname_;
	//SerialPort* port_;
	string manufacturer_;
	string cycleid_;
	string mac_;
	string ytime_;

	map<string, boost::shared_ptr<MeasurePoint> > measurePointList_;

};
}
}


#endif    //__MONITORUNIT_H_