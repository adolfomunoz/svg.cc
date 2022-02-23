#pragma once
#include "node.h"
#include "presentation-attributes.h"

namespace svg {

class Group : public pattern::SelfRegisteringReflectable<Group,Node,PresentationAttributes<Group>> {
public:
    Group() {}
//    auto reflect() { return std::tie(); }
//    auto reflect_names() const { return std::tuple(); }
    static const char* type_name() { return "g"; } 
};
}
