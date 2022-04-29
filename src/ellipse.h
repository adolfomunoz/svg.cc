#include "element.h"
#include "presentation-attributes.h"

namespace svg {

class Ellipse : public pattern::Reflectable<Ellipse,ElementBase,PresentationAttributes<Ellipse>,CoreAttributes<Ellipse>> {
    float _cx, _cy, _rx, _ry;
public:
    Ellipse(float cx = 0.0f, float cy = 0.0f, float rx = 1.0f, float ry = 1.0f) : _cx(cx), _cy(cy), _rx(rx), _ry(ry) {}
    static const char* type_name() { return "ellipse"; } 
    auto reflect() { return std::tie(_cx,_cy,_rx,_ry); }
    auto reflect_names() const { return std::tuple("cx","cy","rx","ry"); }
    float cx() const { return _cx; }
    Ellipse& cx(float f) { _cx=f; return (*this); }
    float cy() const { return _cy; }
    Ellipse& cy(float f) { _cy=f; return (*this); }
    float rx() const { return _rx; }
    Ellipse& rx(float f)  { _rx=f;  return (*this); }
    float ry() const { return _ry; }
    Ellipse& ry(float f)  { _ry=f;  return (*this); }
};

    
}
