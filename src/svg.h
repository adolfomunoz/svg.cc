#pragma once
#include "node.h"
#include "geometry-attributes.h"
#include <iostream>

namespace svg {

class Box {
    float _x, _y, _width, _height;
public:
    Box(float x = 0, float y = 0, float width = 1000, float height = 1000) : _x(x), _y(y),_width(width),_height(height){}
    float x() const { return _x; }
    float y() const { return _y; }
    float width() const { return _width; }
    float height() const { return _height; }
    friend std::istream& operator>>(std::istream& is, Box& box);
};

std::ostream& operator<<(std::ostream& os, const Box& box) {
    os<<box.x()<<" "<<box.y()<<" "<<box.width()<<" "<<box.height();
    return os;
}

std::istream& operator>>(std::istream& is, Box& box) {
    is>>box._x>>box._y>>box._width>>box._height;
    return is;
}

class SVG : public pattern::Reflectable<SVG,Node,GeometryAttributes<SVG>,AspectRatioAttributes<SVG>> {
    std::string xmlns, xmlns_xlink; //Do not modify these, it is for the slylesheet
    std::optional<Box> _viewBox;
public:
    SVG() :
        xmlns("http://www.w3.org/2000/svg"),
        xmlns_xlink("http://www.w3.org/1999/xlink") {}

    auto reflect() { return std::tie(xmlns,xmlns_xlink,_viewBox); }
    auto reflect_names() const { return std::tuple("xmlns","xmlns:xlink","viewBox"); }
    static const char* type_name() { return "svg"; } 
    SVG& viewBox(const Box& b) { _viewBox = b; return *this; }
    SVG& viewBox(float x, float y, float width, float height) { return viewBox(Box(x,y,width,height)); }
    Box viewBox() const { return _viewBox.value_or(Box()); }
    std::string code() const {
        return pattern::xml(*this,
                pattern::xml_reflect_attributes_from_stream | 
                pattern::xml_tag_as_derived);
    }
    void load_from_code(const std::string& content) {
        pattern::load_xml(*this, content);
    }
    bool load(std::istream& in) {
        std::ostringstream sstr;
        sstr << in.rdbuf();
        load_from_code(sstr.str());
        return true;
    }
    bool load(const std::string& filename) {
        std::ifstream in(filename);
        if (in.is_open()) return load(in);
        else return false;
    }
    void save(std::ostream& out) const {
        out << code();
    }
    void save(const std::string& filename) const {
        std::ofstream out(filename);
        save(out);
    }

};
}
