#include "CodecFactory.h"

#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <utils/Logging.h>

#include <list>

using namespace dbdky;
using namespace std;

namespace dbdky
{
namespace detail
{

	string getFullPath()
	{	
    	char current_abs_path[1024];
    	int count;

    	count = ::readlink("/proc/self/exe", current_abs_path, 1024);
    	if (count < 0 || count >= 1024)
    	{
    		LOG_ERROR << "Get ABS PATH ERROR";
    		return "";
    	}

    	current_abs_path[count] = '\0';

    	string filepath(current_abs_path);
    	int st = filepath.rfind("dbdky_gcc");
    	if (st == string::npos)
    	{
    		LOG_ERROR << "GET ABS PATH ERROR:1";
    		return "";
    	}

    	string filepath1 = filepath.substr(0, st);
       	return filepath1;
	}

	list<string> getCodecLibraries()
	{
		list<string> ret;
		string currentpath = getFullPath();

		DIR *dp;
		struct dirent *entry;
		struct stat statbuf;
		if ((dp = ::opendir(currentpath.c_str())) == NULL)
		{
			LOG_ERROR << "Cannot open directory: " << currentpath;
			return ret;
		}

		::chdir(currentpath.c_str());

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

				if (string::npos != filename.find(".so"))
				{
					ret.push_back(filename);
				}
				else
				{
					continue;
				}
			}
		}
      
        ::chdir("..");
		::closedir(dp);

		return ret;
	}
}
}

namespace dbdky
{
namespace gcc
{
CodecFactory* CodecFactory::instance_ = NULL;

CodecFactory* CodecFactory::getInstance()
{
	if (instance_ == NULL)
	{
		instance_ = new CodecFactory();
	}

	return instance_;
}

CodecFactory::CodecFactory()
{
	list<string> libs = dbdky::detail::getCodecLibraries();
	string abspath = dbdky::detail::getFullPath();

	if (libs.empty() || (abspath.empty()))
	{
		LOG_INFO << "No libraries found";
		return;
	}

	list<string>::const_iterator itr;
	for (itr = libs.begin(); itr != libs.end(); itr++)
	{
		string libname(abspath);
		libname += *itr;
		void * hGetInstanceHandle = ::dlopen(libname.c_str(), RTLD_NOW);
		if (NULL == hGetInstanceHandle)
		{
			LOG_ERROR << "Load library: " << libname << " faild";
			continue;
		}

		GetInstanceFuncPtr pfunc = (GetInstanceFuncPtr)(::dlsym(hGetInstanceHandle, "getInstance"));
		if (NULL == pfunc)
		{
			LOG_ERROR << "Load getInstance handle from " << libname << " failed";
			continue;
		}

		int index1 = (*itr).find("lib");
		int index2 = (*itr).find(".so");

		if ((string::npos != index1) && (string::npos != index2) && (index1 < index2))
		{
			string pname = (*itr).substr(index1 + 3, index2 - index1 - 3);
			boost::shared_ptr<CodecBase> pcodecbase(pfunc());
			if (pcodecbase)
			{
				codecList_[pname] = pcodecbase;
			}
		}
	}


}

CodecBase* CodecFactory::getCodec(string protocolname)
{
	if (protocolname.empty())
	{
		return NULL;
	}

	map<string, boost::shared_ptr<CodecBase> >::const_iterator itr;

	itr = codecList_.find(protocolname);
	if (itr != codecList_.end())
	{
		return itr->second.get();
	}
	return NULL;
}
}
}