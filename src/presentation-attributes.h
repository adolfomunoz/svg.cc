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
    
    std::optional<Length> _stroke_width;
public:
    PresentationAttributes() {}
    
	T& stroke_width(const Length& l) noexcept {
        _stroke_width = l; return t(); 
	}
    const Length& stroke_width() const noexcept { return _stroke_width.value_or(1); }
    
    auto reflect() { return std::tie(_stroke_width); }
    auto reflect_names() const { return std::tuple("stroke-width"); }
};

}