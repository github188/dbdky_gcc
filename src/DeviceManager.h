#ifndef __DEVICEMANAGER_H_
#define __DEVICEMANAGER_H_

#include <boost/noncopyable.hpp>
#include <string>

#include <boost/shared_ptr.hpp>
#include <vector>

#include "SerialPort.h"
#include "MeasurePoint.h"
#include "MeasureUnit.h"
#include "param.h"

using namespace std;

namespace dbdky
{
namespace gcc
{
typedef struct ComConfig
{
    string name_;
    string desc_;
    int portname_;
    int databits_;
    int stopbits_;
    int parity_;
    int totaltimeout_;
    int intervaltimeout_;
    int inbuffersize_;
    int outbuffersize_;

} ComConfig;

class DeviceManager : boost::noncopyable
{
public:
    static boost::shared_ptr<DeviceManager> getInstance();
	DeviceManager(string deviceTotalName = "./DeviceTotal_v1.0.xml", string deviceConfigPath ="./");
	~DeviceManager();
    void update(string deviceTotalName = "./DeviceTotal_v1.0.xml", string deviceConfigPath ="./");
    vector<string> getFiles() const
    {
        return configfiles_;
    }
private:
    static boost::shared_ptr<DeviceManager> instance_;
    string devicetotalname_;
    string deviceconfigpath_;

    void getConfigFileListByPath();

    vector<string> configfiles_;
};
}	
}


#endif    //__DEVICEMANAGER_H_