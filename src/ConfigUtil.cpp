#include "ConfigUtil.h"
#include "Default_defs.h"

#include <utils/Timestamp.h>
#include <utils/Logging.h>

#include <boost/lexical_cast.hpp>

#include "tinyxml.h"


static const string confFileName = "cma_conf.conf";

namespace dbdky
{
namespace port
{
boost::shared_ptr<ConfUtil> ConfUtil::instance_;
boost::shared_ptr<ConfUtil> ConfUtil::getInstance()
{
    if (!instance_.get())
    {
        instance_ = boost::shared_ptr<ConfUtil>(new ConfUtil);
    }

    return instance_;
}

ConfUtil::ConfUtil()
  : systemTick_(DEFAULT_SYSTEMTICK),
    heartBeatTick_(DEFAULT_HEARTBEATTICK),
    uploadMoniDataTick_(DEFAULT_UPLOADMONIDATATICK),
    dbPath_(DEFAULT_DBPATH),
    dbUser_(DEFAULT_DBUSER),
    dbPasswd_(DEFAULT_DBPASSWD),
    dbName_(DEFAULT_DBNAME),
    cmaid_(DEFAULT_CMAID),
    localIP_(DEFAULT_LOCALIP),
    proxyIP_(DEFAULT_PROXYIP),
    proxyPort_(DEFAULT_PORT),
    lastUploadTime_(Timestamp::now().microSecondsSinceEpoch())
{
    updateConfigs();
    dumpConfigs();
}

uint16_t ConfUtil::getSystemTick() const
{
    return systemTick_;
}

uint32_t ConfUtil::getHeartbeatTick() const
{
    return heartBeatTick_;
}

uint32_t ConfUtil::getUploadMoniDataTick() const
{
    return uploadMoniDataTick_;
}

void ConfUtil::updateConfigs()
{
    TiXmlElement* element = NULL;
    TiXmlNode *node = NULL;
    TiXmlNode *inside = NULL;

    TiXmlDocument doc(confFileName);
    doc.LoadFile();
    
    if (doc.Error() && doc.ErrorId() == TiXmlBase::TIXML_ERROR_OPENING_FILE)
    {
        LOG_ERROR << "ERROR";
        return;
    }

    
    node = doc.FirstChild("configs");   
    if ((NULL == node) || !(element = node->ToElement()))
    {
        LOG_ERROR << "Parse config file error.";
        return;
    }

    inside = element->FirstChild("item");
    
    while (inside)
    {
        TiXmlElement *tmpElement = inside->ToElement();
        if (NULL == tmpElement)
        {
            LOG_WARN << "Bad config item";
            inside = element->NextSibling();
            continue;
        }

        string itemName(tmpElement->Attribute("name"));
        string itemValue(tmpElement->Attribute("value"));

        if ("local_ip" == itemName)
        {
            localIP_ = itemValue;
        }
        else if ("proxy_ip" == itemName)
        {
            proxyIP_ = itemValue;
        }
        else if ("proxy_port" == itemName)
        {
            uint16_t tmpProxyPort;
            try
            {
                tmpProxyPort = boost::lexical_cast<uint16_t>(itemValue);
                proxyPort_ = tmpProxyPort; 
            }
            catch (boost::bad_lexical_cast& e)
            {
                LOG_WARN << "Parse item fail.";
            }
        }
        else if ("system_tick" == itemName)
        {
            uint16_t tmpSystemTick;

            try 
            {
                tmpSystemTick = boost::lexical_cast<uint16_t>(itemValue);
                systemTick_ = tmpSystemTick;
            }
            catch (boost::bad_lexical_cast& e)
            {
                LOG_WARN << "Parse item fail.";
            }
        }
        else if ("heartbeat_tick" == itemName)
        {
            uint32_t tmpHeartbeatTick;

            try
            {
                tmpHeartbeatTick = boost::lexical_cast<uint32_t>(itemValue);
                
                heartBeatTick_ = tmpHeartbeatTick;
            }
            catch (boost::bad_lexical_cast& e)
            {
                LOG_WARN << "Parse item fail.";
            }
        }
        else if ("uploadmonidata_tick" == itemName)
        {
            uint32_t tmpUploadMoniDataTick;

            try
            {
                tmpUploadMoniDataTick = boost::lexical_cast<uint32_t>(itemValue);
 
                uploadMoniDataTick_ = tmpUploadMoniDataTick;
            }
            catch (boost::bad_lexical_cast& e)
            {
                LOG_WARN << "Parse item fail.";
            }
        }
        else if ("db_path" == itemName)
        {
            dbPath_ = itemValue;
        }
        else if ("db_user" == itemName)
        {
            dbUser_ = itemValue;
        }
        else if ("db_passwd" == itemName)
        {
            dbPasswd_ = itemValue;
        }
        else if ("db_name" == itemName)
        {
            dbName_ = itemValue;
        }
        else if ("cmaid" == itemName)
        {
            cmaid_ = itemValue;
        }
        else if ("last_upload" == itemName)
        {
            uint64_t tmpLastUpload;

            try
            {
                lastUploadTime_ = boost::lexical_cast<uint64_t>(itemValue);
                lastUploadTime_ = tmpLastUpload;
            }
            catch (boost::bad_lexical_cast& e)
            {
                LOG_WARN << "Parse item fail.";
            }
        }
        else
        {
            LOG_WARN << "Unknown config item found.";
            inside = tmpElement->NextSibling();
            continue;
        }

        inside = tmpElement->NextSibling();
    }
}

void ConfUtil::dumpConfigs() const
{
    LOG_INFO << "*********ConfUtil************";
    LOG_INFO << "systemTick: " << systemTick_;
    LOG_INFO << "heartBeatTick: " << heartBeatTick_;
    LOG_INFO << "uploadMoniDataTick: " << uploadMoniDataTick_;
    LOG_INFO << "dbPath: " << dbPath_;
    LOG_INFO << "dbUser: " << dbUser_;
    LOG_INFO << "dbPasswd: " << dbPasswd_;
    LOG_INFO << "dbName: " << dbName_;
    LOG_INFO << "cmaid: " << cmaid_;
    LOG_INFO << "localIP: " << localIP_;
    LOG_INFO << "proxyIP: " << proxyIP_;
    LOG_INFO << "proxyPort: " << proxyPort_;
    LOG_INFO << "lastUploadTime: " << Timestamp(lastUploadTime_).toFormattedString();
    LOG_INFO << "************ConfUtil***********************";
}

void ConfUtil::saveItem2File(const string& itemName, const string& itemValue)
{
    //TODO:
    LOG_INFO << "========" << itemName << "     " << itemValue;
    LOG_INFO << "confFileName:" << confFileName;
    TiXmlDocument doc(confFileName);
    TiXmlNode* node = NULL;
    TiXmlNode* inside = NULL;
    TiXmlElement* element = NULL;
    TiXmlElement* itemElement = NULL;

    doc.LoadFile();
    
    if (doc.Error() && doc.ErrorId() == TiXmlBase::TIXML_ERROR_OPENING_FILE)
    {
        LOG_ERROR << "ERROR";
        return;
    }
 
    node = doc.FirstChild("configs");
    
    if ((NULL == node) || !(element = node->ToElement()))
    {
        LOG_ERROR << "ERROR";
        return;
    }

    inside = element->FirstChild("item");
    
    while(inside)
    {
        itemElement = inside->ToElement();
     //   if (!itemElement)
       // if (itemElement)
       // {
       //     inside = itemElement->NextSibling();
       //     continue;
       // }

        string iName = itemElement->Attribute("name");
        string iValue = itemElement->Attribute("value");

        if (itemName == iName)
        {
            itemElement->SetAttribute("value", itemValue.c_str());
            break;
        }
        else
        {
            inside = itemElement->NextSibling();
            continue;
        }
    }

    doc.SaveFile();
}
}
}
