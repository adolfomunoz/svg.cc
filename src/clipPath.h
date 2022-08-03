#pragma once
#include "node.h"


namespace svg {

class ClipPath : public pattern::Reflectable<ClipPath,Node,CoreAttributes<ClipPath>> {
public:
    ClipPath() {}
//    auto reflect() { return std::tie(); }
//    auto reflect_names() const { return std::tuple(); }
    static const char* type_name() { return "clipPath"; } 
};
}
