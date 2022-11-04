#pragma once
#include <patterns/patterns.h>
#include "length.h"
#include "font-weight.h"

namespace svg {
    
template<typename T>
class TextPresentationAttributes : public pattern::Reflectable<TextPresentationAttributes<T>> {
	constexpr T& t() noexcept { return *static_cast<T*>(this); } 
	constexpr const T& t() const noexcept { return *static_cast<const T*>(this); } 
    
    std::optional<Length> _font_size;
    std::optional<FontWeight> _font_weight;
    std::optional<std::string> _font_family;
public:
    TextPresentationAttributes() {}
    
	T& font_size(const Length& l) noexcept {
        _font_size = l; return t(); 
	}
    Length font_size() const noexcept { return _font_size.value_or(16); }
    bool has_font_size() const noexcept { return bool(_font_size); }

    T& font_weight(const FontWeight& fw) noexcept {
        _font_weight = fw;
    }
    T& font_weight(int value) noexcept {
        _font_weight = FontWeightValue(value);
    }
    FontWeight font_weight() const noexcept { return _font_weight.value_or(FontWeightNamedbold()); }
    bool has_font_weight() const noexcept { return bool(_font_weight); }

	T& font_family(const std::string& f) noexcept {
        _font_family = f; return t(); 
	}
    std::string font_family() const noexcept { return _font_family.value_or(""); }
    bool has_font_family() const noexcept { return bool(_font_family); }

    auto reflect() { return std::tie(_font_size,_font_weight,_font_family); }
    auto reflect_names() const { return std::tuple("font-size","font-weight","font-family"); }
};

}
