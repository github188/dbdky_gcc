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
	boost::shared_ptr<DeviceManager> DeviceManager::getInstance()
	{
		if (!instance_.get())
		{
			instance_ = boost::shared_ptr<DeviceManager>(new DeviceManager());
		}

		return instance_;
	}

	DeviceManager::DeviceManager(string deviceTotalName, string deviceConfigPath)
		: devicetotalname_(deviceTotalName),
		  deviceconfigpath_(deviceConfigPath)
	{
		getConfigFileListByPath();
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
}
}
