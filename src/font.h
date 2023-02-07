#pragma once 
#include <iostream>
#include <patterns/patterns.h>

namespace svg {
    
class FontWeightBase : public pattern::SelfRegisteringReflectableBase {
public:
    virtual std::string to_string() const = 0;
    virtual int value() const = 0;
};

class FontWeight : public pattern::Pimpl<FontWeightBase> {
public:
    using pattern::Pimpl<FontWeightBase>::Pimpl;
    using pattern::Pimpl<FontWeightBase>::operator=;
    std::string to_string() const override {
        return this->impl()->to_string();
    }
    int value() const override {
        return this->impl()->value();
    }
};

class FontWeightValue : public pattern::Reflectable<FontWeightValue, FontWeightBase> {
    int value_;
public:
    FontWeightValue(int v = 400) : value_(v) {}
    static const char* type_name() { return "font-weight-value"; } 
    int value() const override { return value_; }
    std::string to_string() const override {
        std::stringstream sstr;
        sstr<<value();
        return sstr.str();
    }
};

//It needs empty constructor for propper registration
#define NAMED_FONT_WEIGHT(NAME,V) \
    inline class FontWeightNamed##NAME : public pattern::Reflectable<FontWeightNamed##NAME, FontWeightBase> {\
        public: \
            FontWeightNamed##NAME () {}\
            static const char* type_name() { return #NAME; } \
            std::string to_string() const override { return #NAME; } \
            int value() const override { \
                return V; \
            }\
    } NAME;
    
// NAMED_FONT_WEIGHT(normal,400)   Conflict
NAMED_FONT_WEIGHT(bold,700)

inline std::ostream& operator<<(std::ostream& os, const FontWeight& c) {
    os<<c.to_string(); 
    return os;
};

inline std::istream& operator>>(std::istream& is, FontWeight& c) {
    if ((is.peek()>='0') && (is.peek()<='9')) {
        int value;
        is>>value;
        c = FontWeightValue(value);
    } else {
        std::string token; 
        is>>token;
        c.set_type(token);
    }
    return is;
}

class FontStyleBase : public pattern::SelfRegisteringReflectableBase {  };
class FontStyle : public pattern::Pimpl<FontStyleBase> {
public:
    using pattern::Pimpl<FontStyleBase>::Pimpl;
    using pattern::Pimpl<FontStyleBase>::operator=;
};
//It needs empty constructor for propper registration
#define NAMED_FONT_STYLE(NAME) \
    inline class FontStyleNamed##NAME : public pattern::Reflectable<FontStyleNamed##NAME, FontStyleBase> {\
        public: \
            FontStyleNamed##NAME () {}\
            static const char* type_name() { return #NAME; } \
    } NAME;
    
// NAMED_FONT_STYLE(normal)   Conflict
NAMED_FONT_STYLE(italic)
NAMED_FONT_STYLE(oblique)

inline std::ostream& operator<<(std::ostream& os, const FontStyle& c) {
    os<<c.type(); 
    return os;
};
inline std::istream& operator>>(std::istream& is, FontStyle& c) {
    std::string token; 
    is>>token;
    c.set_type(token);
    return is;
}

inline class FontNormal : public pattern::Reflectable<FontNormal, FontWeightBase, FontStyleBase> {
    public:
        FontNormal() {}
        static const char* type_name() { return "normal"; } 
        std::string to_string() const override { return "normal"; } 
        int value() const override { 
            return 400;
        }
} normal;

}
