#ifndef __DBHELPER_H_
#define __DBHELPER_H_

#include <string>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/function.hpp>

#include <utils/Mutex.h>

#include <mysql_connection.h>
#include <mysql_driver.h>
#include <statement.h>

using namespace sql;
using std::string;
namespace dbdky
{
namespace cma_server 
{
class DBHelper : boost::noncopyable
{
public:
    DBHelper(const string& dbpath,
             const string& dbuser,
             const string& dbpasswd,
             const string& dbname);
    ~DBHelper();

    enum StateE {kNoErr, kConnectErr, kStateErr, kQueryErr, kUnknownErr };

    StateE getLastError() const;

    bool connect();
    void release();

    bool isConnected() const;
    
    boost::shared_ptr<ResultSet> query(const string& sqlstr);

private:
    mutable dbdky::MutexLock mutex_;
    mutable dbdky::MutexLock connMutex_;
    StateE error_;
    bool isConnected_;
    const string dbPath_;
    const string dbUser_;
    const string dbPasswd_;
    const string dbName_;
    boost::scoped_ptr<mysql::MySQL_Driver> driver_;
    boost::scoped_ptr<Connection> conn_;
    boost::scoped_ptr<Statement> statement_;
};
}
}

#endif    //__DBHELPER_H_
