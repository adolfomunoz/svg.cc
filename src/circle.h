#include "element.h"

namespace svg {

class Circle : public pattern::SelfRegisteringReflectable<Circle,ElementBase> {
    float cx, cy, r;
public:
    Circle(float cx = 0.0f, float cy = 0.0f, float r = 1.0f) : cx(cx), cy(cy), r(r) {}
    static const char* type_name() { return "circle"; } 
    auto reflect() { return std::tie(cx,cy,r); }
    auto reflect_names() const { return std::tuple("cx","cy","r"); }
};

    
}