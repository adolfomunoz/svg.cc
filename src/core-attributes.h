#pragma once
#include <patterns/patterns.h>
#include "style.h"
#include <iomanip>

namespace svg {
    
template<typename T>
class CoreAttributes : public pattern::Reflectable<CoreAttributes<T>> {
    //This needs to be dynamic because the reflectable thing. Not my proudest moment but this is a
    //requirement of the pattern library with reflection and inheritance
	constexpr T& t() noexcept { return *static_cast<T*>(this); } 
	constexpr const T& t() const noexcept { return *static_cast<const T*>(this); } 
   
    inline static unsigned int next_id = 0; 
    std::string _id;
    std::optional<std::string> _lang;
    std::optional<int> _tabindex;
    std::optional<StyleAttr> _style;
public:
    CoreAttributes() {
        std::stringstream sstr;
        sstr<<T::type_name()<<std::setfill('0')<<std::setw(5)<<(++next_id);
        _id = sstr.str();
    }

    T& id(const std::string& s) noexcept { _id = s; return t(); }
    const std::string& id() const noexcept { return _id; }
    T& lang(const std::string& s) noexcept { _lang = s; return t(); }
    std::string lang() const noexcept { return _lang.value_or(std::string()); }
    T& tabindex(int i) noexcept { _tabindex = i; return t(); }
    int tabindex() const noexcept { return _tabindex.value_or(-1); }
    T& style(const StyleAttr& s) { _style = s; return t(); }
    T& style(StyleAttr&& s) { _style = std::forward<StyleAttr>(s); return t(); }
    StyleAttr style() const { return _style.value_or(StyleAttr()); }
     
    auto reflect() { return std::tie(_id,_lang,_tabindex,_style); }
    auto reflect_names() const { return std::tuple("id","lang","tabindex","style"); }
};

}
