#pragma once
#include <patterns/patterns.h>
#include "length.h"
#include "color.h"

namespace svg {
    
template<typename T>
class PresentationAttributes : public pattern::Reflectable<PresentationAttributes<T>> {
	constexpr T& t() noexcept { return *static_cast<T*>(this); } 
	constexpr const T& t() const noexcept { return *static_cast<const T*>(this); } 
    
    std::optional<Length> _stroke_width;
    std::optional<Color> _stroke, _fill;
    std::optional<float> _opacity, _fill_opacity;
public:
    PresentationAttributes() {}
    
	T& stroke_width(const Length& l) noexcept {
        _stroke_width = l; return t(); 
	}
    Length stroke_width() const noexcept { return _stroke_width.value_or(1); }
    
    auto reflect() { return std::tie(_stroke_width,_stroke,_fill,_opacity,_fill_opacity); }
    auto reflect_names() const { return std::tuple("stroke-width","stroke","fill","opacity","fill-opacity"); }
};

}