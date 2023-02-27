#pragma once
#include "element.h"
#include "presentation-attributes.h"
#include "geometry-attributes.h"
#include "graphical-attributes.h"

namespace svg {

class Use : public pattern::Reflectable<Use,ElementBase,
        PresentationAttributes<Use>,GeometryAttributes<Use>,CoreAttributes<Use>,
        GraphicalAttributes<Use>> {
    std::string _href;
public:
    Use(const std::string& href = "") : _href(href) {}
    Use(const svg::Element& element) : _href(std::string("#")+element.get_attribute("id")) {}
    static const char* type_name() { return "use"; } 
    auto reflect() { return std::tie(_href,_href); }
    auto reflect_names() const { return std::tuple("href","xlink:href"); }
//    auto reflect() { return std::tie(_href); }
//    auto reflect_names() const { return std::tuple("href"); }
	Use& href(const std::string& s) noexcept {
        _href = s; return *this; 
	}
	Use& href(const svg::Element& element) noexcept {
        return href(std::string("#")+element.get_attribute("id")); 
	}
    const std::string& href() const noexcept { return _href; }
};

}
