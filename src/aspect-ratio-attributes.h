#pragma once
#include <patterns/patterns.h>



namespace svg {

class PreserveAspectRatioAlignmentBase : public pattern::SelfRegisteringReflectableBase {
public:
    virtual const char* name() const { return object_type_name(); }
    virtual ~PreserveAspectRatioAlignmentBase() {}
};

class PreserveAspectRatioStrategyBase : public pattern::SelfRegisteringReflectableBase {
public:
    virtual const char* name() const { return object_type_name(); }
    virtual ~PreserveAspectRatioStrategyBase() {}
};

class PreserveAspectRatioAlignment : public pattern::Pimpl<PreserveAspectRatioAlignmentBase> {
public:
    using pattern::Pimpl<PreserveAspectRatioAlignmentBase>::Pimpl;
    using pattern::Pimpl<PreserveAspectRatioAlignmentBase>::operator=;

    const char* name() const override { return this->impl()->name(); }
}; 

class PreserveAspectRatioStrategy : public pattern::Pimpl<PreserveAspectRatioStrategyBase> {
public:
    using pattern::Pimpl<PreserveAspectRatioStrategyBase>::Pimpl;
    using pattern::Pimpl<PreserveAspectRatioStrategyBase>::operator=;

    const char* name() const override { return this->impl()->name(); }
}; 

//It needs empty constructor for propper registration
#define PAR_ALIGNMENT(NAME) \
    inline class Aligmnent##NAME : public pattern::Reflectable<Aligmnent##NAME, PreserveAspectRatioAlignmentBase> {\
        public: \
            Aligmnent##NAME () {}\
            static const char* type_name() { return #NAME; } \
    } NAME;

    //It needs empty constructor for propper registration
#define PAR_STRATEGY(NAME) \
    inline class Strategy##NAME : public pattern::Reflectable<Strategy##NAME, PreserveAspectRatioStrategyBase> {\
        public: \
            Strategy##NAME () {}\
            static const char* type_name() { return #NAME; } \
    } NAME;

namespace par {
    PAR_ALIGNMENT(none)
    PAR_ALIGNMENT(xMinYMin)
    PAR_ALIGNMENT(xMidYMin)
    PAR_ALIGNMENT(xMaxYMin)
    PAR_ALIGNMENT(xMinYMid)
    PAR_ALIGNMENT(xMidYMid)
    PAR_ALIGNMENT(xMaxYMid)
    PAR_ALIGNMENT(xMinYMax)
    PAR_ALIGNMENT(xMidYMax)
    PAR_ALIGNMENT(xMaxYMax)
    PAR_STRATEGY(meet)
    PAR_STRATEGY(slice)
};

struct PreserveAspectRatio {
    PreserveAspectRatioAlignment alignment;
    std::optional<PreserveAspectRatioStrategy> strategy;
};

inline std::ostream& operator<<(std::ostream& os, const PreserveAspectRatio& par) {
    os<<par.alignment.name();
    if (par.strategy) os<<" "<<(*par.strategy).name();
    return os;
}

inline std::istream& operator>>(std::istream& is, PreserveAspectRatio& par) {
    std::string alignment, strategy;
    std::getline(is,alignment,' ');
    std::getline(is,strategy,' ');
    par.alignment.set_type(alignment);
    if (strategy.empty()) par.strategy = {};
    else par.strategy = PreserveAspectRatioStrategy(strategy);
    return is;
}
    
template<typename T>
class AspectRatioAttributes : public pattern::Reflectable<AspectRatioAttributes<T>> {
	constexpr T& t() noexcept { return *static_cast<T*>(this); } 
	constexpr const T& t() const noexcept { return *static_cast<const T*>(this); } 
    
    std::optional<PreserveAspectRatio> _preserve_aspect_ratio;
public:
    AspectRatioAttributes() {}
    
	T& preserveAspectRatio(const PreserveAspectRatioAlignment& l) noexcept {
        _preserve_aspect_ratio = PreserveAspectRatio{l,{}};
        return t(); 
	}
    T& preserveAspectRatio(const PreserveAspectRatioAlignment& l, 
                           const PreserveAspectRatioStrategy& s) noexcept {
        _preserve_aspect_ratio = PreserveAspectRatio{l,s};
        return t(); 
	}
    
    auto reflect() { return std::tie(_preserve_aspect_ratio); }
    auto reflect_names() const { return std::tuple("preserveAspectRatio"); }
};

}
