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
    std::remove(name.begin(),name.end(),' ');
    t.set_type(name);
    t.read_params(is);
    if (is.peek() == ')') { char dummy; is>>dummy; }
    return is;
}

//Empty constructor for propper registration
class Scale : public pattern::Reflectable<Scale, TransformBase> {
    std::array<float,2> data; 
public:
    Scale(float v = 1.0f) : data{v,v} { }
    Scale(float x, float y) : data{x,y} { }
    static const char* type_name() { return "scale"; }
    void read_params(std::istream& is) override {
        is>>data[0];
        if (is.peek() == ')') data[1]=data[0];
        else {
            if (is.peek() == ',') { char dummy; is>>dummy;}
            is>>data[1];
        }
    }
    void write_params(std::ostream& os) const override {
        os<<data[0]<<" "<<data[1];
    }        
};

/*
template<std::size_t N>
class NonScaleTransform : public pattern::Reflectable<NonScaleTransform<N>,TransformBase> {
protected:
    std::array<float,N> data;
public:
    NonScaleTransform() { data.fill(0.0f); }

    void read_params(std::istream& is) override {
        for (std::size_t i = 0; (i<N) && (is.peek()!=')'); ++i) {
            is>>data[i];
            if (is.peek() == ',') { char dummy; is>>dummy; }
        }
    }
    void write_params(std::ostream& os) const override {
        for (std::size_t i = 0; i<N; ++i) os<<" "<<data[i];
    }

    ~NonScaleTransform() {}
};
*/

/*
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
*/

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


/*
DECLARE_TRANSFORM(translate,2)
DECLARE_TRANSFORM(rotate,3)
DECLARE_TRANSFORM(skewX,1)  
DECLARE_TRANSFORM(skewY,1)  
*/

template<typename T>
class GraphicalAttributes : public pattern::Reflectable<GraphicalAttributes<T>> {
	constexpr T& t() noexcept { return *static_cast<T*>(this); } 
	constexpr const T& t() const noexcept { return *static_cast<const T*>(this); } 
    
    std::optional<std::list<Transform>> _transform;
public:
    GraphicalAttributes() {}
    
    T& transform(const std::list<Transform>& l) noexcept {
        _transform=l; return t();
    }
    T& transform(std::list<Transform>&& l) noexcept {
        _transform=std::forward<std::list<Transform>>(l); return t();
    }
    std::list<Transform> transform() const noexcept { return _transform.value_or(std::list<Transform>()); }
   
	T& add_transform(const Transform& tr) {
        if (!_transform) _transform = std::list<Transform>();
        (*_transform).push_front(tr);
        return t();
	}
    
    auto reflect() { return std::tie(_transform); }
    auto reflect_names() const { return std::tuple("transform"); }
};

}
