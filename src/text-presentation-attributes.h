#pragma once
#include <patterns/patterns.h>
#include "length.h"

namespace svg {
    
template<typename T>
class TextPresentationAttributes : public pattern::Reflectable<TextPresentationAttributes<T>> {
	constexpr T& t() noexcept { return *static_cast<T*>(this); } 
	constexpr const T& t() const noexcept { return *static_cast<const T*>(this); } 
    
    std::optional<Length> _font_size;
public:
    TextPresentationAttributes() {}
    
	T& font_size(const Length& l) noexcept {
        _font_size = l; return t(); 
	}
    Length font_size() const noexcept { return _font_size.value_or(16); }
    bool has_font_size() const noexcept { return bool(_font_size); }

    auto reflect() { return std::tie(_font_size); }
    auto reflect_names() const { return std::tuple("font-size"); }
};

}
