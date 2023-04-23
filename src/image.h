#pragma once
#include "element.h"
#include "base64.h"
#include "presentation-attributes.h"
#include "geometry-attributes.h"
#include "graphical-attributes.h"
#include "aspect-ratio-attributes.h"

namespace svg {

class Image : public pattern::Reflectable<Image,ElementBase,
        PresentationAttributes<Image>,GeometryAttributes<Image>,CoreAttributes<Image>,
        GraphicalAttributes<Image>,AspectRatioAttributes<Image>> {
    std::string _href;
public:
    Image(const std::string& href = "") : _href(href) {}
    static const char* type_name() { return "image"; } 
    auto reflect() { return std::tie(_href,_href); }
    auto reflect_names() const { return std::tuple("href","xlink:href"); }
//    auto reflect() { return std::tie(_href); }
//    auto reflect_names() const { return std::tuple("href"); }
	Image& href(const std::string& s) noexcept {
        _href = s; return *this; 
	}
    const std::string& href() const noexcept { return _href; }
    Image& embed(const std::string& filename) noexcept {
        std::ifstream stream(filename, std::ios::in | std::ios::binary);
        if (stream.fail()) return (*this);
        else {  
            std::vector<uint8_t> contents((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
            std::stringstream ss;
            ss << "data:image/"<< filename.substr(filename.size()-3)
                << ";base64,"<<base64_encode(contents.data(),contents.size());
            return href(ss.str());
        } 
    } 
};

    
}
