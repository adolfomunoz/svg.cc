#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <string>
#include <iterator>
#include "element.h"




namespace svg {

class Script : public pattern::Reflectable<Script,ElementBase> {
    std::string _type;
    std::string _code;
public:
    Script(const std::string& code = std::string()) :
        _code(code) { }
    static const char* type_name() { return "script"; } 
    auto reflect() { return std::tie(_type,_code); }
    auto reflect_names() const { return std::tuple("type"); }
};



} 