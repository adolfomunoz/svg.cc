#pragma once
#include <patterns/patterns.h>

namespace svg {
class TransformBase : public pattern::SelfRegisteringReflectableBase {
public:
    const char* name() const { return object_type_name(); }
    virtual void read_params(std::istream& is) = 0;
    virtual void write_params(std::ostream& os) const = 0;
};

class Transform : public pattern::Pimpl<TransformBase> {
public:
    using pattern::Pimpl<TransformBase>::Pimpl;
    using pattern::Pimpl<TransformBase>::operator=;

    void read_params(std::istream& is) override {
        this->impl()->read_params(is);
    }
    void write_params(std::ostream& os) const override {
        this->impl()->write_params(os);
    }
};

inline std::ostream& operator<<(std::ostream& os, const Transform& t) {
    os<<t.name()<<"("; 
    t.write_params(os);
    os<<")";
    return os;
}

inline std::istream& operator>>(std::istream& is, Transform& t) {
    std::string name;
    
    std::getline(is,name,'(');
    auto it = std::remove(name.begin(),name.end(),' ');
    t.set_type(std::string(name.begin(),it));
    t.read_params(is);
    if (is.peek() == ')') { char dummy; is>>dummy; }
    return is;
}

//Empty constructor for propper registration
class Scale : public pattern::Reflectable<Scale, TransformBase> {
    float x, y;
public:
    Scale(float v = 1.0f) : x(v), y(v) { }
    Scale(float x, float y) : x(x), y(y) { }
    static const char* type_name() { return "scale"; }
    void read_params(std::istream& is) override {
        is>>x;
        if (is.peek() == ')') y=x;
        else {
            if (is.peek() == ',') { char dummy; is>>dummy;}
            is>>y;
        }
    }
    void write_params(std::ostream& os) const override {
        os<<x<<" "<<y;
    }        
};

//Empty constructor for propper registration
class Translate : public pattern::Reflectable<Translate, TransformBase> {
    float x, y;
public:
    Translate(float x = 0.0f, float y = 0.0f) : x(x), y(y) { }
    static const char* type_name() { return "translate"; }
    void read_params(std::istream& is) override {
        is>>x;
        if (is.peek() == ')') y=0.0f;
        else {
            if (is.peek() == ',') { char dummy; is>>dummy;}
            is>>y;
        }
    }
    void write_params(std::ostream& os) const override {
        os<<x<<" "<<y;
    }        
};

//Empty constructor for propper registration
class Rotate : public pattern::Reflectable<Rotate, TransformBase> {
    float angle,x, y;
public:
    Rotate(float angle = 0.0f, float x = 0.0f, float y = 0.0f) : angle(angle), x(x), y(y) { }
    static const char* type_name() { return "rotate"; }
    void read_params(std::istream& is) override {
        is>>angle;
        if (is.peek() == ')') x=y=0.0f;
        else {
            if (is.peek() == ',') { char dummy; is>>dummy;}
            is>>x;
            if (is.peek() == ',') { char dummy; is>>dummy;}
            is>>y;
        }
    }
    void write_params(std::ostream& os) const override {
        os<<angle<<" "<<x<<" "<<y;
    }        
};

class Matrix : public pattern::Reflectable<Matrix, TransformBase> {
    std::array<float,6> data;
public:
    Matrix() : data{1,0,0,1,0,0} {}
    Matrix(float a, float b, float c, float d, float e, float f) : data{a,b,c,d,e,f} {}
    static const char* type_name() { return "matrix"; }    
    void read_params(std::istream& is) override {
        for (std::size_t i = 0; (i<6); ++i) {
            is>>data[i];
            if (is.peek() == ',') { char dummy; is>>dummy; }
        }
    }
    void write_params(std::ostream& os) const override {
        for (std::size_t i = 0; i<6; ++i) os<<" "<<data[i];
    }
};

class SkewX : public pattern::Reflectable<SkewX, TransformBase> {
    float s;
public:
    SkewX(float s = 0.0f) : s(s) {}
    static const char* type_name() { return "skewX"; }    
    void read_params(std::istream& is) override {
        is>>s;
    }
    void write_params(std::ostream& os) const override {
        os<<s;
    }
};

class SkewY : public pattern::Reflectable<SkewY, TransformBase> {
    float s;
public:
    SkewY(float s = 0.0f) : s(s) {}
    static const char* type_name() { return "skewY"; }    
    void read_params(std::istream& is) override {
        is>>s;
    }
    void write_params(std::ostream& os) const override {
        os<<s;
    }
};

}