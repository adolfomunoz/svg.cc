#pragma once
#include "node.h"

namespace svg {

class Defs : public pattern::Reflectable<Defs,Node> {
public:
    Defs() {}
//    auto reflect() { return std::tie(); }
//    auto reflect_names() const { return std::tuple(); }
    static const char* type_name() { return "defs"; } 
};
}
