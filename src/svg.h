#pragma once
#include "node.h"

namespace svg {

class SVG : public pattern::SelfRegisteringReflectable<SVG,Node> {
    std::string xmlns, xmlns_xlink; //Do not modify these, it is for the slylesheet
public:
    SVG() :
        xmlns("http://www.w3.org/2000/svg"),
        xmlns_xlink("http://www.w3.org/1999/xlink") {}

    auto reflect() { return std::tie(xmlns,xmlns_xlink); }
    auto reflect_names() const { return std::tuple("xmlns","xmlns_xlink"); }
    static const char* type_name() { return "svg"; } 
    std::string code() const {
        return pattern::xml(*this,
                pattern::xml_reflect_attributes_from_stream | 
                pattern::xml_tag_as_derived);
    }
    void load_from_code(const std::string& content) {
        pattern::load_xml(*this, content);
    }
    bool load(const std::string& filename) {
        std::ifstream in(filename);
        if (in.is_open()) {
            std::ostringstream sstr;
            sstr << in.rdbuf();
            load_from_code(sstr.str());
            return true;
        } else return false;
    }
    void save(const std::string& filename) {
        std::ofstream out(filename);
        out << code();
    }

};
}
