#include "element.h"
#include "presentation-attributes.h"
#include "geometry-attributes.h"

namespace svg {

class Rect : public pattern::SelfRegisteringReflectable<Rect,ElementBase,
        PresentationAttributes<Rect>,GeometryAttributes<Rect>> {
    std::optional<Length> _rx, _ry;
public:
    Rect(float x0 = 0.0f, float y0 = 0.0f, float x1 = 1.0f, float y1 = 1.0f) {
        x(std::min(x0,x1));
        y(std::min(y0,y1));
        width(std::abs(x1-x0));
        height(std::abs(y1-y0));
    }
    static const char* type_name() { return "rect"; } 
    auto reflect() { return std::tie(_rx,_ry); }
    auto reflect_names() const { return std::tuple("rx","ry"); }
	Rect& rx(const Length& l) noexcept {
        _rx = l; return *this; 
	}
    Length rx() const noexcept { return _rx.value_or(0); }
	Rect& ry(const Length& l) noexcept {
        _ry = l; return *this; 
	}
    Length ry() const noexcept { return _ry.value_or(0); }
};

    
}