#pragma once
#include <patterns/patterns.h>
#include "length.h"

namespace svg {
    
template<typename T>
class PresentationAttributes : public pattern::Reflectable<PresentationAttributes<T>> {
    //This needs to be dynamic because the reflectable thing. Not my proudest moment but this is a
    //requirement of the pattern library with reflection and inheritance
	constexpr T& t() noexcept { return *static_cast<T*>(this); } 
	constexpr const T& t() const noexcept { return *static_cast<const T*>(this); } 
    
    std::optional<Length> _x,_y,_width,_height;
public:
    PresentationAttributes() {}
    
	T& x(const Length& l) noexcept {
        _x = l; return t(); 
	}
    Length x() const noexcept { return _x.value_or(0); }
	T& y(const Length& l) noexcept {
        _y = l; return t(); 
	}
    Length y() const noexcept { return _y.value_or(0); }
	T& width(const Length& l) noexcept {
        _width = l; return t(); 
	}
    Length width() const noexcept { return _width.value_or(100,LengthUnitPercentage()); }
	T& height(const Length& l) noexcept {
        _height = l; return t(); 
	}
    Length height() const noexcept { return _width.value_or(100,LengthUnitPercentage()); }
    
    auto reflect() { return std::tie(_x,_y,_width,_height); }
    auto reflect_names() const { return std::tuple("x","y","height","width"); }
};

}