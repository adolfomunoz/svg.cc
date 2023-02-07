#pragma once 
#include <iostream>
#include <patterns/patterns.h>

namespace svg {

class TextAnchorBase : public pattern::SelfRegisteringReflectableBase {  };
class TextAnchor : public pattern::Pimpl<TextAnchorBase> {
public:
    using pattern::Pimpl<TextAnchorBase>::Pimpl;
    using pattern::Pimpl<TextAnchorBase>::operator=;
};
//It needs empty constructor for propper registration
#define NAMED_TEXT_ANCHOR(NAME) \
    inline class TextAnchorNamed##NAME : public pattern::Reflectable<TextAnchorNamed##NAME, TextAnchorBase> {\
        public: \
            TextAnchorNamed##NAME () {}\
            static const char* type_name() { return #NAME; } \
    } NAME;

NAMED_TEXT_ANCHOR(start)
//NAMED_TEXT_ANCHOR(middle)  Conflict
NAMED_TEXT_ANCHOR(end)

inline std::ostream& operator<<(std::ostream& os, const TextAnchor& c) {
    os<<c.type(); 
    return os;
};
inline std::istream& operator>>(std::istream& is, TextAnchor& c) {
    std::string token; 
    is>>token;
    c.set_type(token);
    return is;
}

class DominantBaselineBase : public pattern::SelfRegisteringReflectableBase {  };
class DominantBaseline : public pattern::Pimpl<DominantBaselineBase> {
public:
    using pattern::Pimpl<DominantBaselineBase>::Pimpl;
    using pattern::Pimpl<DominantBaselineBase>::operator=;
};
//It needs empty constructor for propper registration
#define NAMED_DOMINANT_BASELINE(NAME) \
    inline class DominantBaselineNamed##NAME : public pattern::Reflectable<DominantBaselineNamed##NAME, DominantBaselineBase> {\
        public: \
            DominantBaselineNamed##NAME () {}\
            static const char* type_name() { return #NAME; } \
    } NAME;
    
NAMED_DOMINANT_BASELINE(hanging)
// NAMED_Dominant_BASELINE(middle) conflict
NAMED_DOMINANT_BASELINE(central)
NAMED_DOMINANT_BASELINE(alphabetic)
NAMED_DOMINANT_BASELINE(mathematical)
NAMED_DOMINANT_BASELINE(ideographic)

inline class DominantBaselineAuto : public pattern::Reflectable<DominantBaselineAuto, DominantBaselineBase> {\
    public: \
        DominantBaselineAuto () {}\
        static const char* type_name() { return "auto"; } \
} _auto;
    


inline std::ostream& operator<<(std::ostream& os, const DominantBaseline& c) {
    os<<c.type(); 
    return os;
};
inline std::istream& operator>>(std::istream& is, DominantBaseline& c) {
    std::string token; 
    is>>token;
    c.set_type(token);
    return is;
}

inline class MiddleMulticlass : public pattern::Reflectable<MiddleMulticlass, TextAnchorBase, DominantBaselineBase> {
    public:
        MiddleMulticlass () {}
        static const char* type_name() { return "middle"; } 
} middle;



}
