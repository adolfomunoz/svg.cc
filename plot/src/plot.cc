#include "../plot.h"
#include "../../src/poly.h"

namespace svg {
namespace plot {

Plot::Plot(const std::list<float>& x, const std::list<float>& y) : x(x),y(y) {}
Plot::Plot(std::list<float>&& x, const std::list<float>& y) : x(std::move(x)), y(y) {}
Plot::Plot(const std::list<float>& x, std::list<float>&& y) : x(x), y(std::move(y)) {}
Plot::Plot(std::list<float>&& x, std::list<float>&& y) : x(std::move(x)), y(std::move(y)) {}
svg::Element Plot::plot(const Transform& xscale, const Transform& yscale) const noexcept {
    svg::Polyline output;
    std::list<float>::const_iterator ix, iy;
    for (ix = x.begin(), iy = y.begin(); (ix != x.end()) && (iy != y.end()); ++ix, ++iy)
        output.add_point(xscale(*ix),yscale(*iy));
    output.fill(none).stroke_width(linewidth()).stroke(color()).opacity(alpha());
    return output;
}
std::array<float,4> Plot::axis() const noexcept {
    if (x.empty() || y.empty()) return std::array<float,4>{0.0f,0.0f,0.0f,0.0f};
    std::array<float,4> a{x.front(),x.front(),y.front(),y.front()};
    std::list<float>::const_iterator ix, iy;
    for (ix = x.begin(), iy = y.begin(); (ix != x.end()) && (iy != y.end()); ++ix, ++iy) {
        if (*ix < a[0]) a[0] = *ix;
        if (*ix > a[1]) a[1] = *ix;
        if (*iy < a[2]) a[2] = *iy;
        if (*iy > a[3]) a[3] = *iy;
    }
    return a;
}

Plot& Plot::linewidth(float f) noexcept { linewidth_=f; return *this; }
float Plot::linewidth() const noexcept { return linewidth_; }
Plot& Plot::alpha(float f) noexcept { alpha_=f; return *this; }
float Plot::alpha() const noexcept { return alpha_; }
Plot& Plot::color(const svg::Color& c) { color_=c; return *this; }
const svg::Color& Plot::color() const { return color_; }
} 
}