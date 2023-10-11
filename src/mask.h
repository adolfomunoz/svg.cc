#pragma once
#include "node.h"
#include "geometry-attributes.h"




namespace svg {

class Mask : public pattern::Reflectable<Mask,Node,CoreAttributes<Mask>,GeometryAttributes<Mask>> {
public:
    Mask() {}
//    auto reflect() { return std::tie(); }
//    auto reflect_names() const { return std::tuple(); }
    static const char* type_name() { return "mask"; } 
};
}
