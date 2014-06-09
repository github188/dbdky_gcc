#ifndef __PARAM_H_
#define __PARAM_H_

#include <string>

using namespace std;

namespace dbdky
{
namespace gcc
{
class Param
{
private:
	string name_;
	string desc_;
	string unit_;
	string precision_;
	string range_;
	string filter_;
	string addresslen_;
	string address_;
	string registernum_;
};
}
}

#endif    //__PARAM_H_