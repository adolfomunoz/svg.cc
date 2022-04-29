#include "element.h"
#include "presentation-attributes.h"

namespace svg {

class Line : public pattern::Reflectable<Line,ElementBase,PresentationAttributes<Line>,CoreAttributes<Line>> {
    Length _x1, _y1, _x2, _y2;
public:
    Line(float x1 = 0.0f, float y1 = 0.0f, float x2 = 1.0f, float y2 = 1.0f) :
        _x1(x1), _y1(y1), _x2(x2), _y2(y2) { }
    static const char* type_name() { return "line"; } 
    auto reflect() { return std::tie(_x1,_y1,_x2,_y2); }
    auto reflect_names() const { return std::tuple("x1","y1","x2","y2"); }
    
	Line& x1(const Length& l) noexcept {
        _x1 = l; return *this; 
	}
    Length x1() const noexcept { return _x1; }
	Line& y1(const Length& l) noexcept {
        _y1 = l; return *this; 
	}
    Length y1() const noexcept { return _y1; }
	Line& x2(const Length& l) noexcept {
        _x2 = l; return *this; 
	}
    Length x2() const noexcept { return _x2; }
	Line& y2(const Length& l) noexcept {
        _y2 = l; return *this; 
	}
    Length y2() const noexcept { return _y2; }

};

    
}
