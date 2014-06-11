#include "DeviceManager.h"

#include <utils/Timestamp.h>
#include <utils/Logging.h>

#include <boost/lexical_cast.hpp>

#include "tinyxml.h"

#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>




namespace dbdky
{
namespace gcc
{
	boost::shared_ptr<DeviceManager> DeviceManager::instance_;
	boost::shared_ptr<DeviceManager> DeviceManager::getInstance(EventLoop* loop)
	{
		if (!instance_.get())
		{
			instance_ = boost::shared_ptr<DeviceManager>(new DeviceManager(loop));
		}

		return instance_;
	}

	DeviceManager::DeviceManager(EventLoop* loop, string deviceTotalName, string deviceConfigPath)
		: devicetotalname_(deviceTotalName),
		  deviceconfigpath_(deviceConfigPath),
		  loop_(loop),
	{
		getConfigFileListByPath();
		load();
	}

	DeviceManager::~DeviceManager()
	{

	}

	void DeviceManager::update(string deviceTotalName, string deviceConfigPath)
	{
		configfiles_.clear();
		devicetotalname_ = deviceTotalName;
		deviceconfigpath_ = deviceConfigPath;
		getConfigFileListByPath();
	}

	void DeviceManager::getConfigFileListByPath()
	{
		if (!configfiles_.empty())
		{
			configfiles_.clear();
		}

		DIR *dp;
		struct dirent *entry;
		struct stat statbuf;
		if ((dp = ::opendir(deviceconfigpath_.c_str())) == NULL)
		{
			LOG_ERROR << "Cannot open directory: " << deviceconfigpath_;
			return;
		}

		::chdir(deviceconfigpath_.c_str());

		while ((entry = ::readdir(dp)) != NULL)
		{
			::lstat(entry->d_name,&statbuf);
			if (S_ISDIR(statbuf.st_mode))
			{
				continue;

			}
			else
			{
				string filename;
				filename += entry->d_name;
				if ((filename == "FilterResult.xml") || (filename == "Comserver.xml")
				 || (filename == "Filters.xml") || (filename == "DeviceTotal_v1.0.xml"))
				{
					continue;
				}

				if (string::npos != filename.find(".xml"))
				{
					configfiles_.push_back(filename);
				}
				else
				{
					continue;
				}
			}
		}
		::chdir("..");
		::closedir(dp);
	}
   
	SerialPort* DeviceManager::getSerialPortByName(const string& name)
	{
		if (name.empty())
		{
			return NULL;
		}

		map<string, boost::shared_ptr<SerialPort> >::const_iterator itr;
		itr = serialPortList_.find(name);
		if (serialPortList_.end() != itr)
		{
			return itr->second.get();
		}

		return NULL;
	}
    
    void DeviceManager::insertSerialPort(const ComConfig& config)
    {
    	if (config.name_.empty())
    	{
    		return;
    	}

    	SerialPort* pSerial = getSerialPortByName(config.name_);
    	if (!pSerial)
    	{
    		boost::shared_ptr<SerialPort> port(new SerialPort(loop_, config));
    		serialPortList_.insert(make_pair<string, boost::shared_ptr<SerialPort> >(config.name_, port));
    	}
    }

    void getFullParams(string deviceid, string id, string paramname, Param& param)
    {
        TiXmlDocument doc(deviceconfigpath_ + devicetotalname_);
        
    }

    void DeviceManager::DeviceManager::load()
    {
    	if (!serialPortList_.empty())
    	{
    		serialPortList_.clear();
    	}

        //TiXmlDocument doc(devicetotalname_);
        //deviceconfigpath_
    	vector<string>::const_iterator itr;
    	char current_abs_path[1024];
    	// if (NULL == ::getcwd(current_abs_path, 512))
    	// {
    	// 	LOG_ERROR << "Get ABS PATH ERROR";
    	// 	return;
    	// }
    	int count;

    	count = ::readlink("/proc/self/exe", current_abs_path, 1024);
    	if (count < 0 || count >= 1024)
    	{
    		LOG_ERROR << "Get ABS PATH ERROR";
    		return;
    	}

    	current_abs_path[count] = '\0';

    	string filepath(current_abs_path);
    	int st = filepath.rfind("dbdky_gcc");
    	if (st == string::npos)
    	{
    		LOG_ERROR << "GET ABS PATH ERROR:1";
    		return;
    	}

    	string filepath1 = filepath.substr(0, st);
      deviceconfigpath_ = filepath1;

    	for (itr = configfiles_.begin(); itr != configfiles_.end(); itr++)
    	{
    		
    		
    		//string filename = "/home/kevin/workspace/dbdky_gcc/build/debug/bin/" + *itr;
    		string filename = filepath1 + *itr;
    		TiXmlDocument doc(filename);
    		TiXmlNode* node = NULL;
    		TiXmlNode* inside = NULL;
    		TiXmlElement* element = NULL;
    		doc.LoadFile();

    		if (doc.Error() && doc.ErrorId()== TiXmlBase::TIXML_ERROR_OPENING_FILE)
    		{
    			LOG_ERROR << "cannot open xml file: " << filename;
    			continue;
    		}
 
    		node = doc.FirstChild("package");
    		if ((NULL == node) || !(element = node->ToElement()))
    		{
    			LOG_ERROR << "Parse config file error: " << *itr;
    			continue;
    		}

    		inside = element->FirstChild("RS485");
    		TiXmlElement* tmpElement = NULL;
    		if (!inside || (NULL == (tmpElement = inside->ToElement())))
    		{
    			LOG_ERROR << "Invalid format";
    			continue;
    		}
    		
    		ComConfig comconfig;

    		string rs485name(tmpElement->Attribute("name"));
    		if (rs485name.empty())
    		{
    			LOG_ERROR << "There is a RS485 without name";
    			continue;
    		}

    		comconfig.name_ = rs485name;
    		comconfig.desc_ = tmpElement->Attribute("desc");

    		TiXmlNode* inside1 = NULL;
    		inside1 = inside->FirstChild();

    		while (inside1)
    		{
    			string elementValue(inside1->ToElement()->GetText());
    			string elementName(inside1->Value());
  			
  				if (elementName == "PortName")
  				{
  					comconfig.portname_ = elementValue;
  				}
  				else if (elementName == "BaudRate")
  				{
  					comconfig.baudrate_ = elementValue;
  				}
  				else if (elementName == "DataBits")
  				{
  					comconfig.databits_ = elementValue;
  				}
  				else if (elementName == "StopBits")
  				{
  					comconfig.stopbits_ = elementValue;
  				}
  				else if (elementName == "Parity")
  				{
  					comconfig.parity_ = elementValue;
  				}
  				else if (elementName == "TotalTimeOut")
  				{
  					comconfig.totaltimeout_ = elementValue;
  				}
  				else if (elementName == "IntervalTimeOut")
  				{
  					comconfig.intervaltimeout_ = elementValue;
  				}
  				else if (elementName == "InBufferSize")
  				{
  					comconfig.inbuffersize_ = elementValue;
  				}
  				else if (elementName == "OutBufferSize")
  				{
  					comconfig.outbuffersize_ = elementValue;
  				}
  				else
  				{
  					LOG_ERROR << "Redundunt element found";
  				}

    			inside1 = inside1->NextSibling();
    		}

    		insertSerialPort(comconfig);

        //Now we instantiate the MonitorUnit & MeasurePoint & Params
        inside = inside->NextSibling();
        while (inside)
        {
            TiXmlElement* unit = inside->ToElement();
            if ((inside->Value() != "MonitorUnit") || (!unit))
            {
                inside = inside->NextSibling();
                continue;
            }

            string monitorUnitName = unit->Attribute("name");
            string monitorUnitInterval = unit->Attribute("interval");
            string monitorProtocolName = unit->Attribute("protocolname");
            string monitorUnitPort = unit->Attribute("port");
            string monitorUnitManufacturer = unit->Attribute("manufacturer");
            string monitorUnitCycleid = unit->Attribute("cycleid");
            string monitorUnitMac = unit->Attribute("mac");
            string monitorUnitYtime = unit->Attribute("ytime");

            SerialPort* pPort = getSerialPortByName(monitorUnitPort);
            if (!pPort)
            {
                inside = inside->NextSibling();
                continue;
            }

            pPort->insertMonitorUnit(monitorUnitName, monitorUnitInterval, 
                  monitorProtocolName, monitorUnitMac, monitorUnitManufacturer,
                  monitorUnitCycleid, monitorUnitYtime);

            TiXmlNode* nodelevel2 = inside->FirstChild();
            TiXmlElement* elelevel2 = NULL;
            if (!nodelevel2)
            {
                inside = inside->NextSibling();
                continue;
            }

            while (nodelevel2)
            {
                elelevel2 = nodelevel2->ToElement();
                if ((nodelevel2->Value() != "MeasurePoint") 
                  || !elelevel2)
                {
                    nodelevel2 = nodelevel2->NextSibling();
                    continue;
                }

                string pointDeviceid = elelevel2->Attribute("deviceid");
                string pointIEDName = elelevel2->Attribute("IEDName");
                string pointid = elelevel2->Attribute("id");
                string pointchecktime = elelevel2->Attribute("checktime");

                MonitorUnit* pUnit = pPort->getMonitorUnitByName(monitorUnitName);
                if (!pUnit)
                {
                    nodelevel2 = nodelevel2->NextSibling();
                    continue;
                }

                pUnit->insertMeasurePoint(pointDeviceid, pointIEDName, pointid, pointchecktime);

                TiXmlNode* nodelevel3 = nodelevel2->FirstChild();
                TiXmlElement* elelevel3 = NULL;

                if (!nodelevel3)
                {
                  nodelevel2 = nodelevel2->NextSibling();
                  continue;
                }

                while (nodelevel3)
                {
                    elelevel3 = nodelevel3->ToElement();
                    if ((nodelevel3->Value() != "param") || !elelevel3)
                    {
                        nodelevel3 = nodelevel3->NextSibling();
                        continue;
                    }

                    string paramName = elelevel3->Attribute("name");

                    Param tmpParam(paramName);
                    getFullParams(pointDeviceid, pointid, paramName, tmpParam);
                    MeasurePoint* pMeasurePoint = pUnit->getMeasurePointByDeviceid(pointDeviceid);
                    pMeasurePoint->insertParam(tmpParam);

                    nodelevel3 = nodelevel3->NextSibling();

                }
            }

            inside = inside->NextSibling();

        }
    	}

    }

    void DeviceManager::dumpSerialPortsInfo() const
    {
        map<string, boost::shared_ptr<SerialPort> >::const_iterator itr;
        for (itr = serialPortList_.begin(); itr != serialPortList_.end(); itr++)
        {
            itr->second->dumpInfo();
        }
    }

    void DeviceManager::start()
    {
        map<string, boost::shared_ptr<SerialPort> >::const_iterator itr;
        for (itr = serialPortList_.begin(); itr != serialPortList_.end(); itr++)
        {
            itr->second->start();
        }
    }

    void DeviceManager::stop()
    {
        map<string, boost::shared_ptr<SerialPort> >::const_iterator itr;
        for (itr = serialPortList_.begin(); itr != serialPortList_.end(); itr++)
        {
            itr->second->stop();
        }
    }
}
}
