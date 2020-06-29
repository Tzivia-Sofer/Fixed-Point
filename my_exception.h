#include <exception>
#include <stdexcept>

class ErrorOverFlow:public std::runtime_error{
public:
    ErrorOverFlow(std::string s):runtime_error(s){}
};
