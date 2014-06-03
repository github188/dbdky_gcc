#include "redisclient.h"
#include <iostream>
#include <boost/date_time.hpp>

using namespace boost;
using namespace std;

shared_ptr<redis::client> connect_client();

int main()
{
	shared_ptr<redis::client> c;
	c = connect_client();
	c->rpush("setxxx", "Hello");
	redis::client::string_vector vals;
	long num = c->get_list("setxxx", vals);

	for (int i = 0; i < vals.size(); i++)
	{
		cout << vals[i] << endl;
	}

	c->set("set003", "abc333");
	string s = c->get("set003");
	cout << s << endl;

	return 0;
}

shared_ptr<redis::client> connect_client()
{
    const char* c_host = getenv("REDIS_HOST");
    string host = "localhost";

    if (c_host)
    {
    	host = c_host;
    }

    return boost::shared_ptr<redis::client>(new redis::client(host));
}