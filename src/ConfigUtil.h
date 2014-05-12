#ifndef __PORT_CONFIGUTIL_H_
#define __PORT_CONFIGUTIL_H_

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/function.hpp>

#include <port/InetAddress.h>
#include <utils/Timestamp.h>

#include <string>

using std::string;

namespace dbdky
{
namespace port
{
class ConfUtil : boost::noncopyable
{
public:
    static boost::shared_ptr<ConfUtil> getInstance();
   
    //boost::function <void(const string&, const string&)> OnUpdateItem; 

    uint16_t getSystemTick() const;
    uint32_t getHeartbeatTick() const;
    uint32_t getUploadMoniDataTick() const;
 
    string getDBPath() const
    {
        return dbPath_;
    }

    string getDBUser() const
    {
        return dbUser_;
    }

    string getDBPasswd() const
    {
        return dbPasswd_;
    }

    string getDBName() const
    {
        return dbName_;
    }

    string getCmaId() const
    {
        return cmaid_;
    }

    string getLocalIP() const
    {
        return localIP_;
    }

    uint64_t getLastUploadTime() const
    {
        return lastUploadTime_;
    }

    string getProxyIP() const
    {
        return proxyIP_;
    }

    uint16_t getProxyPort() const
    {
        return proxyPort_;
    }

    void dumpConfigs() const;

    void saveItem2File(const string&, const string&);
private:
    ConfUtil();

private:
    static boost::shared_ptr<ConfUtil> instance_;
    uint16_t systemTick_;
    uint32_t heartBeatTick_;
    uint32_t uploadMoniDataTick_;
    string dbPath_;
    string dbUser_;
    string dbPasswd_;
    string dbName_;
    string cmaid_;
    string localIP_;
    string proxyIP_;
    uint16_t proxyPort_;
    uint64_t lastUploadTime_;

    void updateConfigs();
};
}
}



#endif    //__CONFIGUTIL_H_
