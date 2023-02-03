#pragma once
#include "axis-scale.h"
#include "../src/element.h"

namespace svg {
namespace plot {

class PlottableBase {
public:
    virtual svg::Element plot(const Transform& xscale, const Transform& yscale) const noexcept = 0;
    virtual std::array<float,4> axis() const noexcept = 0;
    virtual ~PlottableBase() {}
};

class Plottable : public pattern::Pimpl<PlottableBase> {
public:
    using pattern::Pimpl<PlottableBase>::Pimpl;
    using pattern::Pimpl<PlottableBase>::operator=;
    
    svg::Element plot(const Transform& xscale, const Transform& yscale) const noexcept override {
        return impl()->plot(xscale,yscale);
    }

    std::array<float,4> axis() const noexcept override {
        return impl()->axis();
    }
};

}
}