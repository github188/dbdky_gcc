#ifndef __DEVICEMANAGER_H_
#define __DEVICEMANAGER_H_

#include <boost/noncopyable.hpp>
#include <string>

#include <boost/shared_ptr.hpp>
#include <vector>
#include <map>

#include "SerialPort.h"
#include "MeasurePoint.h"
#include "MonitorUnit.h"
#include "param.h"

#include <port/EventLoop.h>



using namespace std;
using namespace dbdky;
using namespace dbdky::port;

namespace dbdky
{
namespace gcc
{


class DeviceManager : boost::noncopyable
{
public:
    static DeviceManager* getInstance(EventLoop* loop);
	DeviceManager(EventLoop* loop, string deviceTotalName = "./DeviceTotal_v1.0.xml", string deviceConfigPath ="./");
	~DeviceManager();
    void update(string deviceTotalName = "./DeviceTotal_v1.0.xml", string deviceConfigPath ="./");
    vector<string> getFiles() const
    {
        return configfiles_;
    }
    SerialPort* getSerialPortByName(const string& name);
    void insertSerialPort(const ComConfig& config);

    void dumpSerialPortsInfo() const;

    void start();

    void stop();

private:
    void load();
    void getFullParams(string deviceid, string id, string paramname, Param& param);
    static DeviceManager* instance_;
    string devicetotalname_;
    string deviceconfigpath_;
    EventLoop* loop_;
    void getConfigFileListByPath();

    vector<string> configfiles_;

    map<string, boost::shared_ptr<SerialPort> > serialPortList_;

};
}	
}


#endif    //__DEVICEMANAGER_H_