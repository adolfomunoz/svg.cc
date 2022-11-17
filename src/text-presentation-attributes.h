#pragma once
#include <patterns/patterns.h>
#include "length.h"
#include "font.h"
#include "text-alignment.h"

namespace svg {
    
template<typename T>
class TextPresentationAttributes : public pattern::Reflectable<TextPresentationAttributes<T>> {
	constexpr T& t() noexcept { return *static_cast<T*>(this); } 
	constexpr const T& t() const noexcept { return *static_cast<const T*>(this); } 
    
    std::optional<Length> _font_size;
    std::optional<FontWeight> _font_weight;
    std::optional<FontStyle> _font_style;
    std::optional<std::string> _font_family;
    std::optional<TextAnchor> _text_anchor;
    std::optional<DominantBaseline> _dominant_baseline;
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
    FontWeight font_weight() const noexcept { return _font_weight.value_or(normal); }
    bool has_font_weight() const noexcept { return bool(_font_weight); }

    T& font_style(const FontStyle& fs) noexcept {
        _font_style = fs;
    }
    FontStyle font_style() const noexcept { return _font_style.value_or(normal); }
    bool has_font_style() const noexcept { return bool(_font_style); }

	T& font_family(const std::string& f) noexcept {
        _font_family = f; return t(); 
	}
    std::string font_family() const noexcept { return _font_family.value_or(""); }
    bool has_font_family() const noexcept { return bool(_font_family); }

    T& text_anchor(const TextAnchor& ta) noexcept {
        _text_anchor = ta;
    }
    TextAnchor text_anchor() const noexcept { return _text_anchor.value_or(start); }
    bool has_text_anchor() const noexcept { return bool(_text_anchor); }

    T& dominant_baseline(const DominantBaseline& db) noexcept {
        _dominant_baseline = db;
    }
    DominantBaseline dominant_baseline() const noexcept { return _dominant_baseline.value_or(_auto); }
    bool has_dominant_baseline() const noexcept { return bool(_dominant_baseline); }


    auto reflect() { return std::tie(_font_size,_font_weight,_font_family); }
    auto reflect_names() const { return std::tuple("font-size","font-weight","font-family"); }
};

}
