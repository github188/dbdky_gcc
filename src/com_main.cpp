#include "com_service.h"
#include <utils/Logging.h>
#include <port/EventLoop.h>

using namespace dbdky;
using namespace dbdky::port;
using namespace::dbdky::gcc;


int main(int argc, char** argv)
{
	LOG_INFO << "pid = " << getpid();

	EventLoop loop;

	com_service server(&loop, "gcc");
	server.start();
	loop.loop();
	
	return 0;
}