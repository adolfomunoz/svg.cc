#pragma once
#include "plottable.h"
#include <list>

namespace svg {
namespace plot {

class Plot : public PlottableBase {
    std::list<float> x, y;
public:
    Plot(const std::list<float>& x, const std::list<float>& y);
    Plot(std::list<float>&& x, const std::list<float>& y);
    Plot(const std::list<float>& x, std::list<float>&& y);
    Plot(std::list<float>&& x, std::list<float>&& y);

    svg::Element plot(const Transform& xscale, const Transform& yscale) const noexcept override;
    std::array<float,4> axis() const noexcept override;
};
}
}