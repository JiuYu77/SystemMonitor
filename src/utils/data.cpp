#include "utils/data.h"

// double转string, 并保留n位小数
std::string doubleToString(const double &val, int n)
{ 
    std::string ret_str = std::to_string(val);
    size_t pos = ret_str.find('.');
    if (pos != std::string::npos && ret_str.length() > pos + (n+1)) {
        ret_str = ret_str.substr(0, pos + (n+1));
    }
    return ret_str;
}