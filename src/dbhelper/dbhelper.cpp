#include "dbhelper.h"

#include <utils/Logging.h>

namespace dbdky
{
namespace cma_server 
{
DBHelper::DBHelper(const string& dbpath,
                   const string& dbuser,
                   const string& dbpasswd,
                   const string& dbname)
    : dbPath_(dbpath),
      dbUser_(dbuser),
      dbPasswd_(dbpasswd),
      dbName_(dbname),
      isConnected_(false),
      error_(kNoErr),
      driver_(sql::mysql::get_mysql_driver_instance())
{
    LOG_INFO << "DBHelper ctor with params[" << dbpath << " "
            << dbuser << " " << dbpasswd << " " << dbname
            << "]";
}

DBHelper::~DBHelper()
{
    
    driver_.reset();
}

bool DBHelper::connect()
{
    if (isConnected())
    {
        return true;
    }
    try { 
    	conn_.reset(driver_->connect(dbPath_, dbUser_, dbPasswd_));
    	if (!conn_.get())
    	{
        	{
            	MutexLockGuard lock(mutex_);
            	error_ = kConnectErr;
        	}

        	return false;
        
    	}
  
 	statement_.reset(conn_->createStatement());
    	if (!statement_.get())
    	{
            {
            	MutexLockGuard lock(mutex_);
                error_ = kStateErr;
            }
            LOG_ERROR << "Create statement fail.";
            return false;
        }


        string sqlstr = "use " + dbName_ + ";";
        statement_->execute(sqlstr.c_str());

        {
            MutexLockGuard lock(mutex_);
            error_ = kNoErr;
        }

        {
            MutexLockGuard lock(connMutex_);
            isConnected_ = true;
        }
    }
    catch (std::exception& e)
    {
        LOG_ERROR << e.what();
        
        {
            MutexLockGuard lock(connMutex_);
            isConnected_ = false;
        }

        {
            MutexLockGuard lock(mutex_);
            error_ = kUnknownErr;
        }
    }
    return true;
}



DBHelper::StateE DBHelper::getLastError() const
{
    StateE err;

    {
        MutexLockGuard lock(mutex_);
        err = error_;
    }

    return err;
}

bool DBHelper::isConnected() const
{
    bool ret;
    
    {
        MutexLockGuard lock(connMutex_);
        ret = isConnected_;
    }

    return ret;
}

void DBHelper::release()
{
}

boost::shared_ptr<ResultSet> DBHelper::query(const string& sqlstr)
{
    boost::shared_ptr<ResultSet> result;
    try 
    {
        //ResultSet* res = statement_->executeQuery(sqlstr.c_str());
        result.reset(statement_->executeQuery(sqlstr.c_str()));
      
        {
            MutexLockGuard lock(mutex_);
            error_ = kNoErr;
        }

        return result;
    }
    catch (std::exception& e)
    {
        LOG_ERROR << e.what();
        
        {
            MutexLockGuard lock(mutex_);
            error_ = kQueryErr;
        }
    }

    return result;
}

}
}
