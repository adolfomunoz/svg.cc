#include "element.h"
#include "presentation-attributes.h"
#include "geometry-attributes.h"

namespace svg {

class Image : public pattern::Reflectable<Image,ElementBase,
        PresentationAttributes<Image>,GeometryAttributes<Image>,CoreAttributes<Image>> {
    std::string _href;
public:
    Image(const std::string& href = "") : _href(href) {}
    static const char* type_name() { return "image"; } 
    auto reflect() { return std::tie(_href,_href); }
    auto reflect_names() const { return std::tuple("href","xlink:href"); }
	Image& href(const std::string& s) noexcept {
        _href = s; return *this; 
	}
    const std::string& href() const noexcept { return _href; }
};

    
}
