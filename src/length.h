#pragma once 
#include <iostream>
#include <patterns/patterns.h>

namespace svg {

class LengthUnitBase : public pattern::SelfRegisteringReflectableBase {
public:
    virtual float ratio() const = 0;
    virtual const char* name() const = 0;
    bool is_absolute() const { return ratio()>0.0f; }
};

class LengthUnit : public pattern::Pimpl<LengthUnitBase> {
public:
    using pattern::Pimpl<LengthUnitBase>::Pimpl;
    using pattern::Pimpl<LengthUnitBase>::operator=;
    float ratio() const override { return this->impl()->ratio(); }
    const char* name() const override { return this->impl()->name(); }
};

#define LENGTH_UNIT(CLASS,NAME,RATIO) \
    class LengthUnit##CLASS : public pattern::SelfRegisteringReflectable<LengthUnit##CLASS, LengthUnitBase> {\
        public: \
            static const char* type_name() { return NAME; } \
            const char* name() const override { return NAME; } \
            float ratio() const override { return RATIO; } \
    };

LENGTH_UNIT(None,"",1)
LENGTH_UNIT(Px,"px",1)
LENGTH_UNIT(Em,"em",-1)
LENGTH_UNIT(Ex,"ex",-1)
LENGTH_UNIT(In,"in",96)
LENGTH_UNIT(Cm,"cm",37.795)
LENGTH_UNIT(Mm,"mm",377.95)
LENGTH_UNIT(Pt,"pt",1.33333333)
LENGTH_UNIT(Pc,"pc",16)
LENGTH_UNIT(Percentage,"%",-1)



std::ostream& operator<<(std::ostream& os, const LengthUnit& unit) {
    os<<unit.name(); return os;
};

std::istream& operator>>(std::istream& is, LengthUnit& unit) {
    std::string token; is>>token;
    unit.set_type(token);

    return is;
};

class Length {
	float q;
	LengthUnit u;
public:
    Length(float quantity = 1) : q(quantity), u(LengthUnitNone()) {}
	Length(float quantity, const LengthUnit& unit) : q(quantity), u(unit) { }
	float quantity() const noexcept { return q; }
    const LengthUnit& unit() const noexcept { return u; }

    Length operator+(const Length& that) const noexcept {
        return Length(quantity()+that.quantity()*that.unit().ratio()/unit().ratio(),unit());
    }

    Length operator-(const Length& that) const noexcept {
        return Length(quantity()-that.quantity()*that.unit().ratio()/unit().ratio(),unit());
    }

    Length operator*(float that) const noexcept {
        return Length(quantity()*that,unit());
    }
    
    Length operator/(float that) const noexcept {
        return Length(quantity()/that,unit());
    }

    float operator/(const Length& length) const noexcept {
        return (quantity()*unit().ratio())/(length.quantity()*length.unit().ratio());
    }
 
 
    friend std::istream& operator>>(std::istream& is, Length& length);
};

std::ostream& operator<<(std::ostream& os, const Length& length) {
    os<<length.quantity()<<length.unit();
    return os;
}

std::istream& operator>>(std::istream& is, Length& length) {
    is>>length.q>>length.u;
    return is;
}

}
