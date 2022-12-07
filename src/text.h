#pragma once
#include "element.h"
#include "presentation-attributes.h"
#include "text-presentation-attributes.h"
#include "graphical-attributes.h"
#include "length.h"

namespace svg {

class Text : public pattern::Reflectable<Text,ElementBase,
        TextPresentationAttributes<Text>,PresentationAttributes<Text>,CoreAttributes<Text>,GraphicalAttributes<Text>> {
    Length _x, _y;
    std::string _content;
public:
    Text(std::string_view content = "", float x = 0.0f, float y = 0.0f) :
        _x(x), _y(y), _content(content) { }
    static const char* type_name() { return "text"; } 
    auto reflect() { return std::tie(_x,_y,_content); }
    auto reflect_names() const { return std::tuple("x","y"); }
    
	Text& x(const Length& l) noexcept {
        _x = l; return *this; 
	}
    Length x() const noexcept { return _x; }
	Text& y(const Length& l) noexcept {
        _y = l; return *this; 
	}
    Length y() const noexcept { return _y; }

    Text& content(const std::string_view& s) noexcept {
        _content = s; return (*this);
    }
    const std::string& content() const noexcept { return _content; }

//    void init() { std::cerr<<"Init text: "<<content()<<std::endl; }

};

}