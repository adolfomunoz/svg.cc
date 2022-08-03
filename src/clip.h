#pragma once
#include <patterns/patterns.h>

namespace svg {
class ClipBase : public pattern::SelfRegisteringReflectableBase {
public:
    const char* name() const { return object_type_name(); }
    virtual void read_clip_params(std::istream& is) = 0;
    virtual void write_clip_params(std::ostream& os) const = 0;
};

class Clip : public pattern::Pimpl<ClipBase> {
public:
    using pattern::Pimpl<ClipBase>::Pimpl;
    using pattern::Pimpl<ClipBase>::operator=;

    void read_clip_params(std::istream& is) override {
        this->impl()->read_clip_params(is);
    }
    void write_clip_params(std::ostream& os) const override {
        this->impl()->write_clip_params(os);
    }
};

inline std::ostream& operator<<(std::ostream& os, const Clip& c) {
    os<<c.name()<<"("; 
    c.write_clip_params(os);
    os<<")";
    return os;
}

inline std::istream& operator>>(std::istream& is, Clip& c) {
    std::string name;
    
    std::getline(is,name,'(');
    std::remove(name.begin(),name.end(),' ');
    c.set_type(name);
    c.read_clip_params(is);
    if (is.peek() == ')') { char dummy; is>>dummy; }
    return is;
}

class Url : public pattern::Reflectable<Url, ClipBase> {
    std::string id;
public:
    Url(std::string_view id = "") : id(id) {}
    void read_clip_params(std::istream& is) override {
        char dummy;
        is>>dummy;
        std::getline(is,id,')');
    }
    void write_clip_params(std::ostream& os) const override {
        os<<"#"<<id;
    } 
    static const char* type_name() { return "url"; }   
};

}