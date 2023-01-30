#include "axis-scale.h"
#include "../src/element.h"

namespace svg {
namespace plot {

class PlottableBase {
public:
    virtual svg::Element plot(const Scale& xscale, const Scale& yscale) const noexcept = 0;
};

class Plottable : public pattern::Pimpl<PlottableBase> {
public:
    using pattern::Pimpl<PlottableBase>::Pimpl;
    using pattern::Pimpl<PlottableBase>::operator=;
    
    svg::Element plot(const Scale& xscale, const Scale& yscale) const noexcept override {
        return impl()->plot(xscale,yscale);
    }
};

}
}