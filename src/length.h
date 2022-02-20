#pragma once 
#include <iostream>

namespace svg {

enum class LengthUnit {
    none, em, ex, px, in, cm, mm, pt, pc, percentage
};

std::ostream& operator<<(std::ostream& os, LengthUnit unit) {
    switch (unit) {
        case LengthUnit::none : os<<"";   break;
        case LengthUnit::em   : os<<"em"; break;
        case LengthUnit::ex   : os<<"ex"; break;
        case LengthUnit::px   : os<<"px"; break;
        case LengthUnit::in   : os<<"in"; break;
        case LengthUnit::cm   : os<<"cm"; break;
        case LengthUnit::mm   : os<<"mm"; break;
        case LengthUnit::pt   : os<<"pt"; break;
        case LengthUnit::pc   : os<<"pc"; break;
        case LengthUnit::percentage   : os<<"%"; break;
    }
    return os;
};

std::istream& operator>>(std::istream& is, LengthUnit& unit) {
    std::string token; is>>token;
    if (token == "") unit = LengthUnit::none;
    else if (token == "em") unit = LengthUnit::em;
    else if (token == "ex") unit = LengthUnit::ex;
    else if (token == "px") unit = LengthUnit::px;
    else if (token == "in") unit = LengthUnit::in;
    else if (token == "cm") unit = LengthUnit::cm;
    else if (token == "mm") unit = LengthUnit::mm;
    else if (token == "pt") unit = LengthUnit::pt;
    else if (token == "pc") unit = LengthUnit::pc;
    else if (token == "%") unit = LengthUnit::percentage;

    return is;
};

class Length {
	float q;
	LengthUnit u;
public:
	Length(float quantity = 1, const LengthUnit& unit = LengthUnit::none) : q(quantity), u(unit) { }
	float quantity() const { return q; }
    LengthUnit unit() const { return u; }
    
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
