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
    std::optional<std::list<float>> _stroke_dasharray;
public:
    PresentationAttributes() {}
    
	T& stroke_width(const Length& l) noexcept {
        _stroke_width = l; return t(); 
	}
    Length stroke_width() const noexcept { return _stroke_width.value_or(1); }
    bool has_stroke_width() const noexcept { return bool(_stroke_width); }
    
    T& fill(const Color& c) noexcept {
        _fill = c; return t();
    }
    Color fill() const noexcept { return _fill.value_or(black); }
    T& stroke(const Color& c) noexcept {
        _stroke = c; return t();
    }
    Color stroke() const noexcept { return _stroke.value_or(black); }
    T& opacity(float f) noexcept { _opacity = f; return t(); }
    float opacity() const noexcept { return _opacity.value_or(1.0f); }
    T& fill_opacity(float f) noexcept { _fill_opacity = f; return t(); }
    float fill_opacity() const noexcept { return _fill_opacity.value_or(1.0f); }
    T& stroke_dasharray(const std::list<float>& l) noexcept { _stroke_dasharray=l; return t(); }
    std::list<float> stroke_dasharray() const noexcept { return _stroke_dasharray.value_or(std::list<float>()); }

    auto reflect() { return std::tie(_stroke_width,_stroke,_fill,_opacity,_fill_opacity,_stroke_dasharray); }
    auto reflect_names() const { return std::tuple("stroke-width","stroke","fill","opacity","fill-opacity","stroke-dasharray"); }
};

}
