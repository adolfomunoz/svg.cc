#include "element.h"
#include "presentation-attributes.h"
#include "core-attributes.h"

namespace svg {

class Circle : public pattern::Reflectable<Circle,ElementBase,PresentationAttributes<Circle>,CoreAttributes<Circle>> {
    float _cx, _cy, _r;
public:
    Circle(float cx = 0.0f, float cy = 0.0f, float r = 1.0f) : _cx(cx), _cy(cy), _r(r) {}
    static const char* type_name() { return "circle"; } 
    auto reflect() { return std::tie(_cx,_cy,_r); }
    auto reflect_names() const { return std::tuple("cx","cy","r"); }
    float cx() const { return _cx; }
    Circle& cx(float f) { _cx=f; return (*this); }
    float cy() const { return _cy; }
    Circle& cy(float f) { _cy=f; return (*this); }
    float r() const { return _r; }
    Circle& r(float f)  { _r=f;  return (*this); }
};

    
}
