#pragma once 
#include <iostream>
#include <patterns/patterns.h>

namespace svg {
    
class ColorBase : public pattern::SelfRegisteringReflectableBase {
public:
    virtual std::string to_string() const = 0;
    virtual std::tuple<float,float,float> rgb() const = 0;
};

class Color : public pattern::Pimpl<ColorBase> {
public:
    using pattern::Pimpl<ColorBase>::Pimpl;
    using pattern::Pimpl<ColorBase>::operator=;
    std::string to_string() const override {
        return this->impl()->to_string();
    }
    std::tuple<float,float,float> rgb() const override {
        return this->impl()->rgb();
    }
};

//We are ignoring percentage-based RGB, for now
class ColorRGB : public pattern::Reflectable<ColorRGB, ColorBase> {
    std::tuple<float,float,float> rgb_;
public:
    ColorRGB(float r = 0.0f, float g = 0.0f, float b = 0.0f) : rgb_(r,g,b) {}
    ColorRGB(const Color& color) : rgb_(color.rgb()) {}
    static const char* type_name() { return "rgb"; } 
    std::string to_string() const override {
        std::stringstream sstr;
        sstr<<"rgb("<<std::get<0>(rgb_)<<", "<<std::get<1>(rgb_)<<", "<<std::get<2>(rgb_)<<")";
        return sstr.str();
    }
    std::tuple<float,float,float> rgb() const override { return rgb_; }
};

inline ColorRGB rgb(float r, float g, float b) noexcept {
    return ColorRGB(r,g,b);
}

inline ColorRGB hsv(float h, float s, float v) {
	h*=3.0f/180.0; //In 6 sectors (h is in degrees)
	int i = h; //floor of h;
	float f = h-i; //Decimals part;
	float p = v*(1.0f-s);
	float q = v*(1.0f-s*f);
	float t = v*(1.0f-s*(1.0f-f));
	switch (i){
		case 0: return rgb(255.0f*v,255.0f*t,255.0f*p);
		case 1: return rgb(255.0f*q,255.0f*v,255.0f*p);
		case 2: return rgb(255.0f*p,255.0f*v,255.0f*t);
		case 3: return rgb(255.0f*p,255.0f*q,255.0f*t);
		case 4: return rgb(255.0f*t,255.0f*p,255.0f*v);
		default: //5
			return rgb(255.0f*v,255.0f*p,255.0f*q);
	}
}

class ColorHex : public pattern::Reflectable<ColorHex, ColorBase> {
    char hex[6];
    static constexpr float hex_value(char c) {
        if ((c>='0') && (c<='9')) return float(c-'0');
        else if ((c>='A') && (c<='F')) return float(c-'A')+10.0f;
        else if ((c>='a') && (c<='f')) return float(c-'a')+10.0f;
        else return 0.0f;      
    }
public:
    ColorHex(const std::string& h = "") : hex{'0','0','0','0','0','0'} {
        std::size_t i = 0;
        if (!h.empty() && (h[0] == '#')) ++i;
        for (std::size_t  j = 0; (j<6) && (i<h.length()); ++i, ++j) {
            hex[j] = h[i];
            if (h.length()==4) hex[++j] = h[i];
        }
    }
    
    static const char* type_name() { return "hex"; }
    std::string to_string() const override {
        std::stringstream sstr;
        sstr<<"#"; 
        for (std::size_t j=0;j<6;++j) sstr<<hex[j];
        return sstr.str();
    }
    std::tuple<float,float,float> rgb() const override { 
        return std::tuple<float,float,float>(
            16.0f*hex_value(hex[0])+hex_value(hex[1]),
            16.0f*hex_value(hex[2])+hex_value(hex[3]),
            16.0f*hex_value(hex[4])+hex_value(hex[5]));
    }
};

//It needs empty constructor for propper registration
#define NAMED_COLOR(NAME,R,G,B) \
    inline class ColorNamed##NAME : public pattern::Reflectable<ColorNamed##NAME, ColorBase> {\
        public: \
            ColorNamed##NAME () {}\
            static const char* type_name() { return #NAME; } \
            std::string to_string() const override { return #NAME; } \
            std::tuple<float,float,float> rgb() const override { \
                return std::tuple<float,float,float>(R,G,B); \
            }\
    } NAME;
    
//Gathering named colors from https://upload.wikimedia.org/wikipedia/commons/e/e7/SVG1.1_Color_Swatch.svg

NAMED_COLOR(red,255,0,0)
NAMED_COLOR(yellow,255,255,0)
NAMED_COLOR(green,0,128,0)
NAMED_COLOR(blue,0,0,255)
NAMED_COLOR(orange,255,165,0)
NAMED_COLOR(magenta,255,0,255)
NAMED_COLOR(purple,128,0,128)
NAMED_COLOR(pink,255,192,203)
NAMED_COLOR(lime,0,255,0)
NAMED_COLOR(white,255,255,255)
NAMED_COLOR(black,0,0,0)
NAMED_COLOR(gray,128,128,128)
NAMED_COLOR(cyan,0,255,255)

NAMED_COLOR(none,-1,-1,-1)


inline std::ostream& operator<<(std::ostream& os, const Color& c) {
    os<<c.to_string(); 
    return os;
};

inline std::istream& operator>>(std::istream& is, Color& c) {
    std::string token; 
    is>>token;
    if (!token.empty()) {
        if (token[0] == '#') c = ColorHex(token);
        else if ((token.length()>7) && (token.substr(0,3) == "rgb")) {
            char dummy; float r, g, b;
            std::stringstream rgb_stream(token.substr(5));
            rgb_stream>>r>>dummy>>g>>dummy>>b;
            c = ColorRGB(r,g,b);
        }
        else c.set_type(token);
    }
    return is;
}

inline ColorRGB operator+(const Color& c1, const Color& c2) noexcept {
    auto t1 = c1.rgb(); auto t2 = c2.rgb();
    return ColorRGB(std::get<0>(t1)+std::get<0>(t2),
                    std::get<1>(t1)+std::get<1>(t2),
                    std::get<2>(t1)+std::get<2>(t2));
}

inline ColorRGB operator*(const Color& c, float f) noexcept {
    auto t = c.rgb(); 
    return ColorRGB(std::get<0>(t)*f,
                    std::get<1>(t)*f,
                    std::get<2>(t)*f);
}

inline ColorRGB operator*(float f, const Color& c) noexcept {
    return c*f;
}


}
