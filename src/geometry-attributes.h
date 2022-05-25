#pragma once
#include <patterns/patterns.h>
#include <optional>
#include "length.h"

namespace svg {

    
template<typename T>
class GeometryAttributes : public pattern::Reflectable<GeometryAttributes<T>> {
    
	constexpr T& t() noexcept { return *static_cast<T*>(this); } 
	constexpr const T& t() const noexcept { return *static_cast<const T*>(this); } 
    
    std::optional<Length> _x,_y,_width,_height;
    
public:
    GeometryAttributes() {}
    
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
    Length width() const noexcept { return _width.value_or(Length(100,LengthUnitPercentage())); }
	T& height(const Length& l) noexcept {
        _height = l; return t(); 
	}
    Length height() const noexcept { return _width.value_or(Length(100,LengthUnitPercentage())); }
    
    auto reflect() { return std::tie(_x,_y,_width,_height); }
    auto reflect_names() const { return std::tuple("x","y","height","width"); }
};

}
