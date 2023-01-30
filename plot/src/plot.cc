#include "../plot.h"

namespace svg {
namespace plot {

Scale& SVGPlot::set_xscale(const Scale& s) {
    xscale_ = s; return xscale_;
}
Scale& SVGPlot::set_yscale(const Scale& s) {
    yscale_ = s; return yscale_;
}
Scale& SVGPlot::set_xscale(const std::string& s) {
    xscale_.set_type(s); return xscale_;
}
Scale& SVGPlot::set_yscale(const std::string& s) {
    yscale_.set_type(s); return yscale_;
}
const Scale& SVGPlot::xscale() const noexcept { return xscale_; }
const Scale& SVGPlot::yscale() const noexcept { return yscale_; }

SVGPlot& SVGPlot::xscale(const Scale& s) {
    set_xscale(s); return (*this);
}
SVGPlot& SVGPlot::yscale(const Scale& s) {
    set_yscale(s); return (*this);
}
SVGPlot& SVGPlot::xscale(const std::string& s) {
    set_xscale(s); return (*this);
}
SVGPlot& SVGPlot::yscale(const std::string& s) {
    set_yscale(s); return (*this);
}


}
}