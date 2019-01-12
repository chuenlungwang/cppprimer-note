#include <iostream>
#include <sstream>
#include "ShowBackStageCls.h"

std::string ShowBackStageCls::prettyName()
{
    std::ostringstream os;
    os << this->_id << "(" <<  this->_objName << ")";
    return os.str();
}

ShowBackStageCls::ShowBackStageCls(std::string objName):
    _objName(objName)
{
    this->_id = ++ShowBackStageCls::ref;
    std::cout << prettyName()
              << ": ShowBackstageCls() default constructor"
              << std::endl;
}

ShowBackStageCls::ShowBackStageCls(const ShowBackStageCls &rsh)
{
    this->_id = ++ShowBackStageCls::ref;
    this->_objName = rsh._objName;
    std::cout << prettyName()
              << ": ShowBackstageCls(const ShowBackstageCls&) copy constructor"
              << std::endl;
}

ShowBackStageCls::~ShowBackStageCls()
{
    std::cout << prettyName()
              << ": ShowBackstageCls() destructor"
              << std::endl;
}

int ShowBackStageCls::ref = 0;
