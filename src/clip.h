#pragma once
#include <patterns/patterns.h>
#include "length.h"
#include "geometry-attributes.h"

namespace svg {
class ClipBase : public pattern::SelfRegisteringReflectableBase {
public:
    virtual const char* name() const { return object_type_name(); }
    virtual void read_clip_params(std::istream& is) = 0;
    virtual void write_clip_params(std::ostream& os) const = 0;
};

class Clip : public pattern::Pimpl<ClipBase> {
public:
    using pattern::Pimpl<ClipBase>::Pimpl;
    using pattern::Pimpl<ClipBase>::operator=;

    const char* name() const override { return this->impl()->name(); }

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
    auto it = std::remove(name.begin(),name.end(),' ');
    c.set_type(std::string(name.begin(),it));
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

class Inset : public pattern::Reflectable<Inset, ClipBase,GeometryAttributes<Inset>> {
public:
    Inset() {}
    Inset(Length top, Length right, Length bottom, Length left) {
        x(left).y(top).width(right-left).height(bottom-top);
    }

    void read_clip_params(std::istream& is) override {
        Length top, right, bottom, left;
        is>>top>>right>>bottom>>left;
        x(left).y(top).width(right-left).height(bottom-top);
    }
    void write_clip_params(std::ostream& os) const override {
        os<<y()<<" "<<(x()+width())<<" "<<(y()+height())<<" "<<x();
    }
    static const char* type_name() { return "inset"; }   
};

}