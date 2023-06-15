#include "color.h"
#include <sstream>

svg::Color color_from_string(const std::string& s) {
    if (s=="r") return svg::red;
    else if (s=="g") return svg::green; //Note that this color might not be the same as matplotlib
    else if (s=="b") return svg::blue;
    else if (s=="c") return svg::cyan; //Note that this color might not be the same as maplotlib's
    else if (s=="m") return svg::magenta; //Note that this color might not be the same as maplotlib's
    else if (s=="y") return svg::yellow; //Note that this color might not be the same as maplotlib's
    else if (s=="k") return svg::black;
    else if (s=="w") return svg::white;
    else if (s[0]=='#') return svg::ColorHex(s);
    else if (s[0]=='(') {
        char dummy; float r, g, b;
        std::stringstream rgb_stream(s.substr(1));
        rgb_stream>>r>>dummy>>g>>dummy>>b;
        return svg::ColorRGB(255.0f*r,255.0f*g,255.0f*b);
    } else { //Named colors go here although there seems to be a problem when it does not exist
        svg::Color c; c.set_type(s); return c;
    }
}