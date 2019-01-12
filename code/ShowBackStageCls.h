#ifndef __SHOW_BACKSTAGE_CLS__
#define __SHOW_BACKSTAGE_CLS__

#include <string>

class ShowBackStageCls {
public:
    ShowBackStageCls(const std::string &objName = "default");
    ShowBackStageCls(const ShowBackStageCls &rsh);
    ~ShowBackStageCls();
private:
    std::string prettyName();
    std::string _objName;
    int _id;
    int static ref;
};

#endif
